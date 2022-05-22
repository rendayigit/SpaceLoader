#ifndef USER_H
#define USER_H

#include <QtCore/QtCore>
#include <QtNetwork/QHostAddress>
#include <QtNetwork/QTcpSocket>

#include "../../Operations/Cmd/BaseCmd.h"

class User {
   public:
    User(QString userName, QTcpSocket *socket)
        : userName(userName), socketInstances(new QList<QTcpSocket *>()) {
        socketInstances->append(socket);
    };

    QString getUserName() const { return userName; }
    void setUsername(const QString &userName_) { userName = userName_; }

    QList<QTcpSocket *> *getSocketInstances() { return socketInstances; }

    void addSocket(QTcpSocket *socket);
    QString getIp();

   private:
    QString userName;
    QHostAddress ip;
    QList<QTcpSocket *> *socketInstances;
};

#endif  // USER_H
