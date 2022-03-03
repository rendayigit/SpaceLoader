#ifndef SERVER_H
#define SERVER_H

#include <QtCore/QCoreApplication>
#include <QtCore/QPair>
#include <QtCore/QTimer>
#include <QtNetwork/QTcpSocket>

#include "../Client_console/client.h"
#include "../Commands/Server/common/user.h"
#include "../Operations/Operations.h"
#include "../constants.h"
#include "../lib/TCP/server/tcpServer.h"
#include "../lib/YAML/yaml.h"

class Server : public TCPServer, public Operations {
   public:
    static Server *getInstance();
    void onReceived(QTcpSocket *sender, QByteArray message);
    void clientDisconnected(QTcpSocket *clientSocket);
    BaseCmd *getCmd(QString cmdName);
    User *getUser(QString userName);
    User *getUser(QTcpSocket *socket);
    bool isAuthorized(QTcpSocket *sender, QString cmdName);

   private:
    Server();
    void fileTransfer(QTcpSocket *sender, FileTransferCmd *cmd, QByteArray message);
    void parseInternalCmd(QTcpSocket *sender, QByteArray message);
    void connectProcess(QTcpSocket *sender, QProcess *process);

    QList<User *> userList;
    static Server *m_instance;
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
