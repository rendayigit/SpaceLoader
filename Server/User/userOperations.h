#ifndef USEROPERATIONS_H
#define USEROPERATIONS_H

#include <QtCore/QList>
#include <QtCore/QtCore>
#include <QtNetwork/QTcpSocket>

#include "user.h"

class UserOperations {
   public:
    static auto &getInstance() {
        static UserOperations instance;
        return instance;
    }

    void addUser(QTcpSocket *sender, QByteArray message);
    void getUserList(QTcpSocket *sender);
    void removeUser(User *user);
    User *getUser(QString userName);
    User *getUser(QTcpSocket *socket);

   private:
    UserOperations() = default;
    QList<User *> userList;
};

#endif  // USEROPERATIONS_H
