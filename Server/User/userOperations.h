#ifndef USEROPERATIONS_H
#define USEROPERATIONS_H

#include <QtCore/QList>
#include <QtCore/QtCore>
#include <QtNetwork/QHostAddress>
#include <QtNetwork/QTcpSocket>

#include "user.h"

class UserOperations {
   public:
    UserOperations(const UserOperations &) = delete;
    UserOperations &operator=(UserOperations const &) = delete;
    UserOperations(UserOperations &&) = delete;
    UserOperations &operator=(UserOperations &&) = delete;
    ~UserOperations() = default;

    static auto &getInstance() {
        static UserOperations instance;
        return instance;
    }

    void addUser(QTcpSocket *sender, QByteArray message);
    QString getUserList(QTcpSocket *sender);
    void removeUser(QTcpSocket *socket);
    User *getUser(QTcpSocket *socket);
    User *getUser(QString ip);

   private:
    UserOperations() = default;
    QList<User *> userList;
};

#endif  // USEROPERATIONS_H
