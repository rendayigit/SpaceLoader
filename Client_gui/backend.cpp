#include "backend.h"

#include <QTimer>
#include <QtCore/QFile>
#include <QtCore/QThread>

#include "../constants.h"
#include "../lib/Logger/logger.h"
#include "backendOperations.h"
#include "iostream"
#include "listener.h"

Listener* listener;
BackendOperations* backendOp;

Backend::Backend() {
    listener = new Listener(this);
    backendOp = new BackendOperations(this);
}

// void Backend::onReceived(QByteArray message) {
//     parse(message);
//     Backend::buff=message;
//     emit getReceivedText(buff);
// }

void Backend::onReceived(QByteArray message) {
    // Timer set up
    QElapsedTimer t;
    t.start();

    parse(message);

    Backend::buff.append(message);

    // timer
    while (t.elapsed() < 5000) {
        if (buff.size() > 0) {
            flushBuff();
            return;
        }
    }
    flushBuff();
}

void Backend::flushBuff() {
    // if called flush the message
    emit getReceivedText(buff);
    // clear the buff var
    buff.clear();
}

void Backend::onDisconnected() { Log().Error("Disconnected From Server!"); }

void Backend::getTerminalData(QString text) {
    sendCommand(text.mid(text.lastIndexOf(">") + 2, text.size()).toLocal8Bit());
}

void Backend::start(QString ip) { backendOp->start(ip); }

void Backend::selectLogFile(QString fileName) { sendCommand("readLog " + fileName.toLocal8Bit()); }

void Backend::listLogs() { sendCommand("listLogs"); }

void Backend::getUserList() { sendCommand("getUserList"); }

void Backend::fileTransfer(QString localFile, QString serverPath) {
    backendOp->fileTransfer(localFile, serverPath);
}

void Backend::listen(QString ipPort) { backendOp->listen(ipPort); }

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
