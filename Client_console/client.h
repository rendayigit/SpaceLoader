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
    static Client *getInstance();
    void onReceived(QByteArray message);
    void onDisconnected();
    void start(QList<QString> commandArguments);
    void stopAllListeners();

    void fileTransfer(QTcpSocket *sender, FileTransferCmd *cmd, QByteArray message);
    void parseInternalCmd(QTcpSocket *sender, QByteArray message);
    void connectProcess(QTcpSocket *sender, QProcess *process);

    void handleNullCmd(QByteArray message);
    void help(QByteArray message);  // TODO- was private before

   private:
    void listenTo(QString msg);
    void stopListen(QString ipPort);

    /**
     * @brief Continuously read commands from the console
     *
     */
    void loopForCommands();

   public slots:
    void parse(QByteArray message);

   signals:
    void consoleInputReceived(QByteArray input);

   private:
    Client();
    static Client *m_instance;
    QList<Client *> listeners;
};

#endif  // CLIENT_H
