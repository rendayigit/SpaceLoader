#ifndef BACKEND_H
#define BACKEND_H

#include "../lib/TCP/client/tcpClient.h"

class Backend : public TCPClient {
    Q_OBJECT

   public:
    Backend() = default;
    void onReceived(QByteArray message);
    void onDisconnected();

   public slots:
    Q_INVOKABLE void getTerminalData(QString text);
    Q_INVOKABLE void start(QString ip);

   signals:
    void getReceivedText(QString text);
};

#endif  // BACKEND_H