#ifndef TESTCLIENT_H
#define TESTCLIENT_H

#include "../../lib/TCP/client/tcpClient.h"

class TestClient : public TCPClient {
public:
   explicit TestClient();
    void onReceived(QByteArray) override;
    void onDisconnected();
    QString receivedString = "null";
};

#endif // TESTCLIENT_H
