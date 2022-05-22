#include "userOperations.h"

#include "../../common.h"
#include "../../lib/Logger/logger.h"

void UserOperations::addUser(QTcpSocket *sender, QByteArray message) {
    QString username = GetParam(message);
    QHostAddress ip(sender->localAddress().toIPv4Address());

    if (getUser(ip) == nullptr) {
        userList.append(new User(username, sender));
    } else {
        getUser(ip)->addSocket(sender);
    }

    Log().Event(username + " (" + ip.toString() + ") connected.");
}

QString UserOperations::getUserList(QTcpSocket *sender) {
    QString users = "";

    for (int i = 0; i < userList.size(); i++) {
        users += "User #" + QString::number(i + 1);
        users += ": " + userList.at(i)->getUserName();
        users += " (" + userList.at(i)->getIp() + ")\n";
    }

    return users;
}

void UserOperations::removeUser(QTcpSocket *socket) { 
    User *user = getUser(socket);
    QList<QTcpSocket *> *userSockets = user->getSocketInstances();

    userSockets->removeOne(socket);
    
    if(userSockets->isEmpty()) {
        userList.removeOne(user);
        Log().Event(user->getUserName() + " disconnected.");
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

User *UserOperations::getUser(QHostAddress ip) {
    for (auto &user : userList) {
        if(user->getIp() == ip.toString()) {
            return user;
        }
    }

    return nullptr;
}