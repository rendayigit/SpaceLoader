#include "testServer.h"

TestServer::TestServer() {}

void TestServer::onReceived([[maybe_unused]] QTcpSocket *sender, QByteArray message) {
    receivedString = message.data();
}

void TestServer::onDisconnected(QTcpSocket *clientSocket) {
    qDebug() << clientSocket->peerAddress().toIPv4Address() << " disconnected";
}
void TestServer::onConnected(QTcpSocket *clientSocket) {
    qDebug() << clientSocket->peerAddress().toIPv4Address() << " connected";
}
