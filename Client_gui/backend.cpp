#include "backend.h"

#include <sys/stat.h>
#include <fcntl.h>
#include <libssh/libssh.h>
#include <libssh/sftp.h>
#include <unistd.h>

#include <QtCore/QFile>
#include <QtCore/QThread>
#include <fstream>
#include <iostream>
#include <sstream>

#include "../common.h"
#include "../constants.h"
#include "../lib/Logger/logger.h"
#include "egse.h"
#include "listener.h"

Listener* listener;
Egse* egse;

Backend::Backend() {
    listener = new Listener(this);
    egse = new Egse(this);
    localIp = GetLocalIp().last();
}

void Backend::onReceived(QByteArray message) {
    parse(message);
    emit getReceivedText(message);
}

void Backend::onDisconnected() { Log().Error("Disconnected From Server!"); }

void Backend::egseReplier(QString message) { emit egseReply(message); }

void Backend::getTerminalData(QString text) {
    transmit(text.mid(text.lastIndexOf("\n> ") + 3, text.size()).toLocal8Bit());
}

void Backend::start() {
    attemptConnection(serverIp, 1234);
    QThread::msleep(100);
    // TODO - This delay is important. Consider adding same delay for console client

    /* Transmit username */
    QByteArray username = getenv("USERNAME");
    transmit("addUser " + username);
}

void Backend::selectLogFile(QString fileName) { transmit("readLog " + fileName.toLocal8Bit()); }

void Backend::listLogs() { transmit("listLogs"); }

void Backend::getUserList() { transmit("getUserList"); }

void Backend::fileTransfer(QString localFile, QString serverPath) {
    QStringList args;

#ifdef Q_OS_WIN
    QString bash = "cmd.exe";
    args.append("/c");
    args.append("ROBOCOPY");
    localFile = localFile.replace("file:///", "");
#else
    QString bash = "/bin/bash";
    // TODO - args.append("unix copy command here");
    localFile = localFile.replace("file://", "");
#endif

    qDebug() << "1:" << localFile;
    qDebug() << "2:" << serverPath;

    args.append(localFile);
    args.append(serverPath);

    (new QProcess())->start(bash, args);

    transmit("transmit -s " + localFile.toLocal8Bit() + " -d " + serverPath.toLocal8Bit());

    QThread::msleep(10);

    QFile file(localFile);
    file.open(QIODevice::ReadOnly);
    QByteArray fileData = file.readAll();
    if (fileData.isNull()) {
        std::cout << "An error occured: Cannot open the provided file" << std::endl;
        Log().Error("Error opening" + localFile);
        return;
    }
    std::cout << "File size: " << fileData.size() / BYTE_TO_KILOBYTE << "KiloByte" << std::endl;
    int approxfileSize = fileData.size() / (FILETRANSFER_MAX_SINGLE_PACKET_BYTE_SIZE * 10) + 1;
    std::cout << "Progress: ";
    int iteration = 0;
    while (!fileData.isNull()) {
        if (fileData.size() >= FILETRANSFER_MAX_SINGLE_PACKET_BYTE_SIZE) {
            transmit(fileData.mid(0, FILETRANSFER_MAX_SINGLE_PACKET_BYTE_SIZE));
            fileData.remove(0, FILETRANSFER_MAX_SINGLE_PACKET_BYTE_SIZE);
        } else {
            transmit(fileData.mid(0, fileData.size()));
            fileData.clear();
        }
        iteration++;
        QThread::msleep(30);
        if (iteration % approxfileSize == 0) {
            std::cout << ".";
        }
    }
    transmit("#END");

    if (iteration >= 10) {
        std::cout << " Transfer Complete" << std::endl;
    } else {
        std::cout << "An error occured white transferring the file." << std::endl;
        Log().Error("An error occured white transferring file: " + localFile);
    }
}

void Backend::listen(QString ipPort) {
    int idx1 = ipPort.indexOf(" ", QString("Listen").size(), Qt::CaseInsensitive) + 1;

    QString ip;
    QString port;

    int idx2 = ipPort.indexOf(":", idx1, Qt::CaseInsensitive);
    int idx3 = ipPort.size();

    ip = ipPort.mid(idx1, idx2 - idx1);
    port = ipPort.mid(idx2 + 1, idx3 - idx2 - 1);

    listener->attemptConnection(ip, port.toInt());
    listener->setConnected();
}

void Backend::stopListen() { listener->disconnect(); }

void Backend::setServerIp(QString ip) { serverIp = ip; }

QString Backend::getLocalIp() { return localIp; }

void Backend::transmitEgseTc(QString tc) {
    if (egse->isConnected()) {
        egse->buttonCallback(tc);
    } else {
        egseDisconnectedError();
    }
}

void Backend::egseConnect(QString deviceIp, QString devicePort) {
    if (egse->attemptConnection(deviceIp, devicePort.toInt())) {
        emit egseError(false, "Online, Connected");
        egse->setConnected(true);
    } else {
        egseDisconnectedError();
    }
}

void Backend::egseDisconnectedError() {
    QString errorMessage = "ERROR! Not Connected";
    egse->setConnected(false);
    emit egseError(true, errorMessage);
    Log().Error(errorMessage);
}

void Backend::parse(QString text) {
    if (text.contains("Server Logs")) {
        emit clearLogs();
        QStringList logs = text.split("\n");
        logs.removeFirst();
        for (auto& log : logs) {
            emit getLogList(log);
        }
    } else if (text.contains("Reading")) {
        emit getLogText(text);
    } else if (text.contains("User #")) {
        emit getUsers(text);
    }
}

int Backend::fileTransferSsh(QString localFile, QString serverPath) {
    ssh_session session = ssh_new();

    ssh_options_set(session, SSH_OPTIONS_HOST, "192.168.1.2");
    ssh_options_set(session, SSH_OPTIONS_USER, "Administrator");

    int rc = ssh_connect(session);

    if (rc != SSH_OK) {
        QString errorMessage =
            "Error connecting to host: " + QString::fromStdString(ssh_get_error(session));
        qCritical() << errorMessage;
        Log().Error(errorMessage);
        return 0;
    }

    rc = ssh_userauth_password(session, "Administrator", "uyssw");
    if (rc != SSH_OK) {
        QString errorMessage =
            "Error authenticating with password: " + QString::fromStdString(ssh_get_error(session));
        qCritical() << errorMessage;
        Log().Error(errorMessage);
        return 0;
    }

    std::ifstream file(localFile.toStdString(), std::ios::binary);
    if (!file.is_open()) {
        QString errorMessage = "Error opening file";
        qCritical() << errorMessage;
        Log().Error(errorMessage);
        return 0;
    }

    file.seekg(0, file.end);
    size_t fileSize = file.tellg();
    file.seekg(0, file.beg);

    char* buffer = new char[fileSize];
    file.read(buffer, fileSize);

    sftp_session sftp = sftp_new(session);
    if (sftp == nullptr) {
        QString errorMessage =
            "Error allocating SFTP session: " + QString::fromStdString(ssh_get_error(session));
        qCritical() << errorMessage;
        Log().Error(errorMessage);
        return 0;
    }

    rc = sftp_init(sftp);
    if (rc != SSH_OK) {
        QString errorMessage =
            "Error initializing SFTP session: " + QString::fromStdString(ssh_get_error(session));
        qCritical() << errorMessage;
        Log().Error(errorMessage);
        return 0;
    }

    QString fileName = localFile.split("/").last();
    QString remoteFilePath = serverPath + "/" + fileName;

    sftp_file fileHandle =
        sftp_open(sftp, remoteFilePath.toLocal8Bit(), O_WRONLY | O_CREAT | O_TRUNC, S_IRWXU);
    if (fileHandle == nullptr) {
        QString errorMessage = "Error opening file on remote server: " +
                               QString::fromStdString(ssh_get_error(session));
        qCritical() << errorMessage;
        Log().Error(errorMessage);
        return 0;
    }

    rc = sftp_write(fileHandle, buffer, fileSize);
    if (rc < 0) {
        QString errorMessage = "Error writing file to remote server: " +
                               QString::fromStdString(ssh_get_error(session));
        qCritical() << errorMessage;
        Log().Error(errorMessage);
        return 0;
    }

    qInfo() << "File transfer complete";
    Log().Info("File transfer complete");

    sftp_close(fileHandle);
    sftp_free(sftp);
    ssh_disconnect(session);
    ssh_free(session);

    return 1;
}