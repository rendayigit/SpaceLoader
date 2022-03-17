#include "testClient.h"

TestClient::TestClient() { }

void TestClient::onReceived(QByteArray message) {
    receivedString = message.data();
}

void TestClient::onDisconnected() {
    qDebug() << "Disconnected";
}
