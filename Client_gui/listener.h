#ifndef LISTENER_H
#define LISTENER_H

#include "../lib/TCP/client/tcpClient.h"
#include "backend.h"

class Listener : public TCPClient {
   public:
    Listener();
    Listener(const Listener &) = delete;
    Listener &operator=(Listener const &) = delete;
    Listener(Listener &&) = delete;
    Listener &operator=(Listener &&) = delete;
    ~Listener() = default;

    static auto &getInstance() {
        static Listener instance;
        return instance;
    }

    explicit Listener(Backend* backend) { this->backend = backend; };
    void onReceived(QByteArray message) override;
    void disconnect();
    void setConnected() { connected = true; }

   private:
    Backend* backend;
    bool connected = false;
};

#endif  // LISTENER_H