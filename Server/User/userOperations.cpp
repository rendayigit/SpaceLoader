#include "userOperations.h"

#include "../../common.h"
#include "../../lib/Logger/logger.h"

void UserOperations::addUser(QTcpSocket *sender, QByteArray message) {
    QString username = GetParam(message);

    if (getUser(GetIp(sender)) == nullptr) {
        userList.append(new User(username, sender));
    } else {
        getUser(GetIp(sender))->addSocket(sender);
    }

    Log().Event(username + " (" + GetIp(sender) + ") connected.");
}

QString UserOperations::getUserList(QTcpSocket *sender) {
    QString users = "";

    for (int i = 0; i < userList.size(); i++) {
        users += "User #" + QString::number(i + 1);
        users += ": " + userList.at(i)->getUserName();
        users += " (" + GetIp(userList.at(i)->getSocketInstances()->at(0)) + ")\n";
    }

    return users;
}

void UserOperations::removeUser(QTcpSocket *socket) {
    User *user = getUser(socket);
    QList<QTcpSocket *> *userSockets = user->getSocketInstances();

    userSockets->removeOne(socket);

    if (userSockets->isEmpty()) {
        userList.removeOne(user);
        Log().Event(user->getUserName() + " (" + GetIp(socket) + ") disconnected.");
        delete user;
    }
}

User *UserOperations::getUser(QTcpSocket *socket) {
    for (auto &user : userList) {
        for (auto &userSocket : *user->getSocketInstances()) {
            if (userSocket == socket) {
                return user;
            }
        }
    }

    return nullptr;
}

User *UserOperations::getUser(QString ip) {
    for (auto &user : userList) {
        if (Cmp(GetIp(user->getSocketInstances()->at(0)), ip)) {
            return user;
        }
    }

    return nullptr;
}