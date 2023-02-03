#include "tcpServer.h"

#include <QtNetwork/qtcpsocket.h>

TCPServer::TCPServer(QObject *parent) : QObject(parent) {
    server = new QTcpServer();
    server->setMaxPendingConnections(1000);
    connect(server, SIGNAL(newConnection()), this, SLOT(onNewConnection()));
}

void TCPServer::startServer(qint32 portNumber) {
    if (server->listen(QHostAddress::Any, portNumber)) {
        qInfo().noquote() << "Server up, port: " << portNumber;
    } else {
        qInfo().noquote() << server->errorString();
    }
}

void TCPServer::broadcast(QByteArray message) {
    for (auto &socket : socketList) {
        transmit(socket, message);
    }
}

void TCPServer::transmit(QTcpSocket *client, QByteArray data) {
    if (client != nullptr) {
        if (client->state() != QAbstractSocket::SocketState::UnconnectedState) {
            client->write(QDateTime::currentDateTime().toString().toLocal8Bit() + "\n" + data);
            client->waitForBytesWritten();
            client->flush();
        }
    }
}

void TCPServer::onNewConnection() {
    while (server->hasPendingConnections()) {
        QTcpSocket *socket = server->nextPendingConnection();

        QHostAddress serverAddress(socket->localAddress().toIPv4Address());
        QHostAddress peerAddress(socket->peerAddress().toIPv4Address());

        socket->write("Connected to " + serverAddress.toString().toLocal8Bit());
        socket->waitForBytesWritten();
        socket->flush();

        socketList.append(socket);
        qInfo().noquote() << peerAddress.toString() << " connected";

        connect(socket, SIGNAL(readyRead()), this, SLOT(onReadyRead()));
        connect(socket, SIGNAL(disconnected()), this, SLOT(onDisconnected()));

        onConnected(socket);
    }
}

void TCPServer::onDisconnected() {
    for (int i = 0; i < socketList.size(); i++) {
        if (socketList[i]->state() == QAbstractSocket::UnconnectedState) {
            QHostAddress ip4Address(socketList[i]->localAddress().toIPv4Address());
            QString clientAddress = ip4Address.toString();
            qInfo().noquote() << clientAddress << " disconnected";
            onDisconnected(socketList[i]);
            socketList[i]->abort();
            socketList.removeAt(i);
        }
    }
}

void TCPServer::onReadyRead() {
    for (auto &i : socketList) {
        while (i->bytesAvailable() > 0) {
            QByteArray message = i->readAll();
            onReceived(i, message);
        }
    }
}