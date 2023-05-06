#ifndef COMMONCLIENT_H
#define COMMONCLIENT_H

#include "../lib/TCP/client/tcpClient.h"
#include <QtCore/QProcess>

class CommonClient : public TCPClient {
    Q_OBJECT
    
    void onReceived(QByteArray message) override;
    void onDisconnected() override;

    void stopAllListeners();

    void fileTransfer(QString localFile, QString serverPath);
    void parseInternalCmd(QTcpSocket *sender, QByteArray message) /*override*/;
    void connectProcess(QTcpSocket *sender, QProcess *process) /*override*/;

    void handleNullCmd(QByteArray message);
    void help(QByteArray message);  // TODO- was private before

   private:
    void listenTo(QString msg);
    void stopListen(QString ipPort);

   public slots:
    void parse(QByteArray message);

   private:
    // QList<ClientConsole *> listeners;
};

#endif  // COMMONCLIENT_H