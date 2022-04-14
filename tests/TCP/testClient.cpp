#include "testClient.h"

Client::Client() { }

void Client::onReceived(QByteArray message) {
    receivedString = message.data();
}

void Client::onDisconnected() {
    qDebug() << "Disconnected";
}