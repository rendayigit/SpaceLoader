#ifndef TESTSERVER_H
#define TESTSERVER_H

#include <QtNetwork/qtcpsocket.h>
#include "../../lib/TCP/server/tcpServer.h"
#include "../Test_common.h"

class TestServer : public TCPServer {
public:
    TestServer();
    void onReceived(QTcpSocket *sender, QByteArray message);
    void clientDisconnected(QTcpSocket *clientSocket);
    QString receivedString;
};

#endif // TESTSERVER_H