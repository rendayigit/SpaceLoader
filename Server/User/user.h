#ifndef USER_H
#define USER_H

#include <QtCore/QtCore>
#include <QtNetwork/QTcpSocket>

#include "../../Operations/Cmd/BaseCmd.h"

class User {
   public:
    User(QString userName, QTcpSocket *socket) {
        this->userName = userName;
        socketInstances.append(socket);
    };

    QString getUserName() const { return userName; }
    void setUsername(const QString &userName_) { userName = userName_; }

    QList<QTcpSocket *> getSocketInstances() const { return socketInstances; }
    QList<BaseCmd *> getAuthorizedCmds() const { return authorizedCmds; }

    void addSocket(QTcpSocket *socket);
    QString getIp();

    bool checkAuthorization(BaseCmd *cmd);
    bool addAuthorizedCmd(BaseCmd *cmd);
    void removeAuthorizedCmd(BaseCmd *cmd);
    void clearAuthorizations();

   private:
    QString userName = "";
    QList<QTcpSocket *> socketInstances;
    QList<BaseCmd *> authorizedCmds;
};

#endif  // USER_H
