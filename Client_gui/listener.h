#ifndef LISTENER_H
#define LISTENER_H

#include "../lib/TCP/client/tcpClient.h"
#include "backend.h"

class Listener : public TCPClient {
   public:
    explicit Listener(Backend* backend) { this->backend = backend; };
    void onReceived(QByteArray message) override;
    void onDisconnected() override;
    void disconnect();
    void setConnected() { connected = true; }

   private:
    Backend* backend;
    bool connected = false;
};

#endif  // LISTENER_H