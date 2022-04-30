#include "backend.h"
#include <QtCore/qobjectdefs.h>

#include <QtCore/QThread>

#include "../lib/Logger/logger.h"

void Backend::onReceived(QByteArray message) {
    parse(message);
    emit getReceivedText(message);
}

void Backend::onDisconnected() { Log().Error("Disconnected From Server!"); }

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

void Backend::listLogs() {
    sendCommand("listLogs");
}

void Backend::parse(QString text) {
    if (text.contains("Server Logs")) {
        emit clearLogs();
        QStringList logs = text.split("\n");
        logs.removeFirst();
        for (auto &log : logs) {
            emit getLogList(log);
        }
    } else if (text.contains("Reading")) {
        emit getLogText(text);
    }
}
