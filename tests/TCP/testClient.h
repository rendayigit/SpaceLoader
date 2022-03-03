#ifndef CLIENT_H
#define CLIENT_H

#include "../../lib/TCP/client/tcpClient.h"

class Client : public TCPClient {
public:
    explicit Client();
    void onReceived(QByteArray);
    void onDisconnected();
    QString receivedString;
};

#endif // CLIENT_H
