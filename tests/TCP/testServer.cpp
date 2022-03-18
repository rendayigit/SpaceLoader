#include "testServer.h"

TestServer::TestServer() { }

void TestServer::onReceived(QTcpSocket *sender, QByteArray message) {
    receivedString = message.data();
}

void TestServer::clientDisconnected(QTcpSocket *clientSocket) {
    qDebug() << clientSocket->peerAddress().toIPv4Address() << " disconnected";
}

