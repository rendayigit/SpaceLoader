#include "backend.h"

void Backend::getTerminalData(QString text) {
    sendCommand(text.mid(text.lastIndexOf(">") + 2, text.size()).toLocal8Bit());
}

void Backend::onReceived(QByteArray message) {
    std::cout << message.toStdString() << std::endl;

    emit getReceivedText(message);
}

void Backend::onDisconnected() { std::cout << "Disconnected From Server!"; }

void Backend::start(QString ip) {
    attemptConnection(ip, 1234);

    /* Transmit username */
    QByteArray username = getenv("USERNAME");
    sendCommand("addUser " + username);
}