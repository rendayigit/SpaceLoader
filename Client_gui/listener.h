#ifndef LISTENER_H
#define LISTENER_H

#include "../lib/TCP/client/tcpClient.h"
#include "backend.h"

class Listener : public TCPClient {
   public:

    explicit Listener(Backend* backend) { this->backend = backend; };
    void onReceived(QByteArray message);
    void onDisconnected();
    void disconnect();

   private:
    Backend* backend;
};

#endif  // LISTENER_H