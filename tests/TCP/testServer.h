#ifndef SERVER_H
#define SERVER_H

#include <QtNetwork/qtcpsocket.h>
#include "../../lib/TCP/server/tcpServer.h"
#include "../Test_common.h"

class Server : public TCPServer {
public:
    Server();
    void onReceived(QTcpSocket *sender, QByteArray message);
    void clientDisconnected(QTcpSocket *clientSocket);
    QString receivedString;
};

#endif // SERVER_H