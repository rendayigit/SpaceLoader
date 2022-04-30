#ifndef SERVER_H
#define SERVER_H

#include <QtCore/qglobal.h>
#include <QtCore/QCoreApplication>
#include <QtCore/QPair>
#include <QtCore/QTimer>
#include <QtNetwork/QTcpSocket>

#include "../Client_console/client.h"
#include "../Commands/cmd_plugin_interface.h"
#include "../Operations/Operations.h"
#include "../constants.h"
#include "../lib/TCP/server/tcpServer.h"
#include "../lib/YAML/yaml.h"
#include "../path.h"

class Server : public TCPServer, public Operations {
   public:
    // Server(const Server &) = delete;
    // Server &operator=(const Server &) = delete;
    // Server(Server &&) = delete;
    // Server &operator=(Server &&) = delete;
    // ~Server() = default;

    static auto &getInstance() {
        static Server instance;
        return instance;
    }

    void onReceived(QTcpSocket *sender, QByteArray message);
    void clientDisconnected(QTcpSocket *clientSocket);
    BaseCmd *getCmd(QString cmdName);
    bool isAuthorized(QTcpSocket *sender, QString cmdName);

   private:
    Server();
    void fileTransfer(QTcpSocket *sender, QString localFile, QString serverPath);
    void parseInternalCmd(QTcpSocket *sender, QByteArray message);
    void clearUserAuths(QTcpSocket *sender);
    void connectProcess(QTcpSocket *sender, QProcess *process);
    
    QString transferredFileName;
    QString transferredFileLocation;
    QByteArray transferredFileBuffer;
    QTimer *triggerCmdTimer;
    QTimer *authenticationTimer;
    QTcpSocket *fileTransfererSocket;
    bool isFileTransferInProgress;
    bool isServiceAvailable;
    QDateTime formerCurrTime;  // TODO replace with a QTimer
};

#endif  // SERVER_H
