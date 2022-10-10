#ifndef EGSE_H
#define EGSE_H

#include "../lib/TCP/client/tcpClient.h"
#include "backend.h"

class Egse : public TCPClient {
   public:
    explicit Egse(Backend* backend);
    void onReceived(QByteArray message) override;
    void buttonCallback(QString buttonId);
    void onDisconnected() override;

    bool isConnected() const { return connected; }
    void setConnected(bool connected) { this->connected = connected; }

   private:
    Backend* backend;
    void interpret(QByteArray tc);
    void transmitAscii(QByteArray message);
    bool connected;
};

#endif  // EGSE_H
