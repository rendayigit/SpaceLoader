#include "userOperations.h"

#include <QtNetwork/QHostAddress>

#include "../../common.h"
#include "../../lib/Logger/logger.h"

void UserOperations::addUser(QTcpSocket *sender, QByteArray message) {
    QString username = GetParam(message);
    QHostAddress ip(sender->localAddress().toIPv4Address());

    if (getUser(username) == nullptr) {
        userList.append(new User(username, sender));
    } else {
        getUser(username)->addSocket(sender);
    }

    Log().Event(username + " (" + ip.toString() + ") connected.");
}

void UserOperations::getUserList(QTcpSocket *sender) {
    QString users = "";

    for (int i = 0; i < userList.size(); i++) {
        users += "User #" + QString::number(i + 1);
        users += ": " + userList.at(i)->getUserName();
        users += " (" + userList.at(i)->getIp() + ")\n";
    }

    Transmit(sender, users.toLocal8Bit());
}

void UserOperations::removeUser(User *user) { userList.removeOne(user); }

User *UserOperations::getUser(QString userName) {
    for (auto &i : userList) {
        if (i->getUserName() == userName) return i;
    }

    return nullptr;
}

User *UserOperations::getUser(QTcpSocket *socket) {
    for (auto &user : userList) {
        for (auto &userSocket : user->getSocketInstances()) {
            if (userSocket == socket) {
                return user;
            }
        }
    }

    return nullptr;
}