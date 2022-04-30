#ifndef CLIENT_H
#define CLIENT_H

#include <QtConcurrent/QtConcurrent>
#include <QtCore/QCoreApplication>
#include <QtNetwork/QHostAddress>
#include <QtNetwork/QNetworkInterface>
#include <iostream>

#include "../Operations/Operations.h"
#include "../constants.h"
#include "../lib/TCP/client/tcpClient.h"

class Client : public TCPClient, public Operations {
    Q_OBJECT
   public:
    static auto &getInstance() {
        static Client instance;
        return instance;
    }

    void onReceived(QByteArray message);
    void onDisconnected();
    void start(QString ip);
    void stopAllListeners();

    void fileTransfer(QTcpSocket *sender, FileTransferCmd *cmd, QByteArray message);
    void parseInternalCmd(QTcpSocket *sender, QByteArray message);
    void connectProcess(QTcpSocket *sender, QProcess *process);

    void handleNullCmd(QByteArray message);
    void help(QByteArray message);  // TODO- was private before

   private:
    void listenTo(QString msg);
    void stopListen(QString ipPort);

   public slots:
    void parse(QByteArray message);

   signals:
    void consoleInputReceived(QByteArray input);

   private:
    Client();
    QList<Client *> listeners;
};

#endif  // CLIENT_H
