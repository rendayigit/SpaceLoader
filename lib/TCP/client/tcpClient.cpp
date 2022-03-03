#include "tcpClient.h"

TCPClient::TCPClient(QObject *parent) : QObject(parent) {}

bool TCPClient::attemptConnection(QString serverIP, qint32 serverPort) {
    socket = new QTcpSocket(this);
    socket->connectToHost(serverIP, serverPort);

    if (socket->waitForConnected(3000)) {
        connect(socket, SIGNAL(readyRead()), this, SLOT(onReadyRead()));
        connect(socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(onError(QAbstractSocket::SocketError)));
        //        socket->waitForReadyRead(500);
        return true;
    } else {
        qInfo() << socket->errorString();
        return false;
    }
}

void TCPClient::sendCommand(QByteArray data) {
    if (socket->state() == QAbstractSocket::ConnectedState) {
        socket->write(data);
        socket->waitForBytesWritten();
    }
}

void TCPClient::waitForData(qint32 timeout) {
    socket->waitForReadyRead(timeout);
}

void TCPClient::onReadyRead() {
    QByteArray message = socket->readAll();
    onReceived(message);
}

void TCPClient::onError(QAbstractSocket::SocketError socketError) {
    if (socketError == QAbstractSocket::SocketError::RemoteHostClosedError)
        onDisconnected();
}
