#include "backend.h"

#include <QtCore/QThread>

#include "../lib/Logger/logger.h"

void Backend::getTerminalData(QString text) {
    sendCommand(text.mid(text.lastIndexOf(">") + 2, text.size()).toLocal8Bit());
}

void Backend::onReceived(QByteArray message) { emit getReceivedText(message); }

void Backend::onDisconnected() { Log()->Error("Disconnected From Server!"); }

void Backend::start(QString ip) {
    attemptConnection(ip, 1234);
    QThread::msleep(100);

    /* Transmit username */
    QByteArray username = getenv("USERNAME");
    sendCommand("addUser " + username);
}