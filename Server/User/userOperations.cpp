#include "userOperations.h"

#include <QtNetwork/QHostAddress>

#include "../../common.h"
#include "../../lib/Logger/logger.h"

void UserOperations::addUser(QTcpSocket *sender, QByteArray message) {
    message.replace("username ", "");
    QString username = message.mid(0, message.indexOf(" "));
    QHostAddress ip(sender->localAddress().toIPv4Address());

    if (getUser(username) == nullptr)
        userList.append(new User(username, sender));
    else
        getUser(username)->socketInstances.append(sender);

    Log()->Event(username + " (" + ip.toString() + ") connected.");
}

void UserOperations::getUserList(QTcpSocket *sender) {
    QString users = "";

    for (int i = 0; i < userList.size(); i++) {
        QHostAddress ip(userList.at(i)->socketInstances.at(0)->localAddress().toIPv4Address());
        users += "User #" + QString::number(i);
        users += " username: " + userList.at(i)->getUserName() + '\n';
        users += "User #" + QString::number(i);
        users += " ip: " + ip.toString() + '\n';
    }

    Transmit(sender, users.toLocal8Bit());
}

void UserOperations::removeUser(QTcpSocket *socket) {
    userList.removeOne(getUser(socket));
}

User *UserOperations::getUser(QString userName) {
    for (auto &i : userList) {
        if (i->getUserName() == userName) return i;
    }

    return nullptr;
}

User *UserOperations::getUser(QTcpSocket *socket) {
    for (auto &i : userList) {
        for (auto &j : i->socketInstances) {
            if (j == socket) return i;
        }
    }

    return nullptr;
}