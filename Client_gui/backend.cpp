#include "backend.h"

#include <fcntl.h>
#include <libssh/libssh.h>
#include <libssh/sftp.h>
#include <sys/stat.h>
#include <unistd.h>

#include <QtCore/QFile>
#include <QtCore/QThread>
#include <fstream>
#include <iostream>
#include <sstream>

#include "../common.h"
#include "../lib/Logger/logger.h"
#include "egse.h"
#include "listener.h"

Listener *listener;
Egse *egse;

Backend::Backend() : localIp(GetLocalIp().last()) {
    listener = new Listener(this);
    egse = new Egse(this);
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

// TODO - implement to console client as well.
// TODO - Log ssh operations on the server side as well and not just the client side.
int Backend::fileTransfer(QString localFile, QString serverPath) {
    ssh_session session = ssh_new();

    ssh_options_set(session, SSH_OPTIONS_HOST, "192.168.1.2");
    ssh_options_set(session, SSH_OPTIONS_USER, "Administrator");

    int rc = ssh_connect(session);

    if (rc != SSH_OK) {
        Log().Error("Error connecting to host: " + QString::fromStdString(ssh_get_error(session)));
        return -1;
    }

    rc = ssh_userauth_password(session, "Administrator", "uyssw");
    if (rc != SSH_OK) {
        Log().Error("Error authenticating with password: " +
                    QString::fromStdString(ssh_get_error(session)));
        return -1;
    }

    std::ifstream file(localFile.toStdString(), std::ios::binary);
    if (!file.is_open()) {
        Log().Error("Error opening file");
        return -1;
    }

    file.seekg(0, file.end);
    size_t fileSize = file.tellg();
    file.seekg(0, file.beg);

    char* buffer = new char[fileSize];
    file.read(buffer, fileSize);

    sftp_session sftp = sftp_new(session);
    if (sftp == nullptr) {
        Log().Error("Error allocating SFTP session: " +
                    QString::fromStdString(ssh_get_error(session)));
        return -1;
    }

    rc = sftp_init(sftp);
    if (rc != SSH_OK) {
        Log().Error("Error initializing SFTP session: " +
                    QString::fromStdString(ssh_get_error(session)));
        return -1;
    }

    QString fileName = localFile.split("/").last();
    QString remoteFilePath = serverPath + "/" + fileName;

    sftp_file fileHandle =
        sftp_open(sftp, remoteFilePath.toLocal8Bit(), O_WRONLY | O_CREAT | O_TRUNC, S_IRWXU);
    if (fileHandle == nullptr) {
        Log().Error("Error opening file on remote server: " +
                    QString::fromStdString(ssh_get_error(session)));
        return -1;
    }

    rc = sftp_write(fileHandle, buffer, fileSize);
    if (rc < 0) {
        Log().Error("Error writing file to remote server: " +
                    QString::fromStdString(ssh_get_error(session)));
        return -1;
    }

    Log().Info("File transfer complete");

    sftp_close(fileHandle);
    sftp_free(sftp);
    ssh_disconnect(session);
    ssh_free(session);

    return 0;
}