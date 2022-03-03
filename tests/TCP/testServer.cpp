#include "testServer.h"

Server::Server() { }

void Server::onReceived(QTcpSocket *sender, QByteArray message) {
    receivedString = message.data();
}

void Server::clientDisconnected(QTcpSocket *clientSocket) {
    qDebug() << clientSocket->peerAddress().toIPv4Address() << " disconnected";
}

