#include "backend.h"

#include <QtConcurrent/QtConcurrent>
#include <fcntl.h>
#include <libssh/libssh.h>
#include <libssh/sftp.h>
#include <sys/stat.h>

#include <QtCore/QThread>
#include <fstream>

#include "../common.h"
#include "../lib/Logger/logger.h"
#include "egse.h"
#include "listener.h"
#include "../lib/Ssh/ssh.h"

Listener* listener;
Egse* egse;

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
    QtConcurrent::run([=]() {
        emit setTransferProgress(true);

        SSH::fileTransfer(localFile, serverPath);

        emit setTransferProgress(false);
        return 0;
    });

    return 0;
}
