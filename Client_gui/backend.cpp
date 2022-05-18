#include "backend.h"

#include <QHostInfo>
#include <QNetworkInterface>
#include <QTcpSocket>
#include <QtCore/QFile>
#include <QtCore/QThread>
#include <QtNetwork/QHostAddress>

#include "../constants.h"
#include "../lib/Logger/logger.h"
#include "iostream"
#include "listener.h"


Listener* listener;
QString ip = "";

Backend::Backend() { listener = new Listener(this); }

void Backend::onReceived(QByteArray message) {
    parse(message);
    emit getReceivedText(message);
}

void Backend::onDisconnected() { Log().Error("Disconnected From Server!"); }
void Backend::setServerIP(QString text) { ip = text; }

QString Backend::getServerIP() { return ip; }

QString Backend::getUserIP() {
    // QHostAddress ip(getSocket()->localAddress().toIPv4Address()); //this function gives error
    // return ip.toString(); 
    QHostAddress address;
    const QHostAddress& localhost = QHostAddress(QHostAddress::LocalHost);
    for (const QHostAddress& address : QNetworkInterface::allAddresses()) {
        if (address.protocol() == QAbstractSocket::IPv4Protocol && address != localhost) {
            qDebug() << "ip:" << address.toString();
            return address.toString();
        }
    }
    return "";
}

void Backend::getTerminalData(QString text) {
    sendCommand(text.mid(text.lastIndexOf(">") + 2, text.size()).toLocal8Bit());
}

void Backend::start(QString ip) {
    attemptConnection(ip, 1234);
    QThread::msleep(100);

    /* Transmit username */
    QByteArray username = getenv("USERNAME");
    sendCommand("addUser " + username);
}

void Backend::selectLogFile(QString fileName) { sendCommand("readLog " + fileName.toLocal8Bit()); }

void Backend::listLogs() { sendCommand("listLogs"); }

void Backend::getUserList() { sendCommand("getUserList"); }

void Backend::fileTransfer(QString localFile, QString serverPath) {
    // localFile = localFile.replace('"', "");
    // serverPath = serverPath.replace('"', "");

    localFile = localFile.replace("file:///", "");

    qDebug() << "1:" << localFile;
    qDebug() << "2:" << serverPath;

    sendCommand("transmit -s " + localFile.toLocal8Bit() + " -d " + serverPath.toLocal8Bit());

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
            sendCommand(fileData.mid(0, FILETRANSFER_MAX_SINGLE_PACKET_BYTE_SIZE));
            fileData.remove(0, FILETRANSFER_MAX_SINGLE_PACKET_BYTE_SIZE);
        } else {
            sendCommand(fileData.mid(0, fileData.size()));
            fileData.clear();
        }
        iteration++;
        QThread::msleep(30);
        if (iteration % approxfileSize == 0) {
            std::cout << ".";
        }
    }
    sendCommand("#END");

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
