#ifndef LISTENER_H
#define LISTENER_H

#include "../lib/TCP/client/tcpClient.h"
#include "backend.h"

class Listener : public TCPClient {
   public:
    explicit Listener(Backend *backend) { this->backend = backend; };
    ~Listener() override = default;

    Listener(const Listener &) = delete;
    Listener &operator=(Listener const &) = delete;
    Listener(Listener &&) = delete;
    Listener &operator=(Listener &&) = delete;

    void onReceived(QByteArray message) override;
    void disconnect();
    void setConnected() { connected = true; }

   private:
    Backend *backend;
    bool connected = false;
};

#endif  // LISTENER_H