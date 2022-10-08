#ifndef EGSE_H
#define EGSE_H

#include "../lib/TCP/client/tcpClient.h"
#include "backend.h"

class Egse : public TCPClient {
   public:
    explicit Egse(Backend* backend);
    void onReceived(QByteArray message) override;
    void sendTc(QByteArray tc);

   private:
    Backend* backend;
};

#endif  // EGSE_H