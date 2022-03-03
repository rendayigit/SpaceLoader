#include <QtCore/QtPlugin>
#include <QtNetwork/QHostAddress>

#include "../lib/Logger/logger.h"
#include "common/serverCommon.h"
#include "common/user.h"

static User *getUser(QTcpSocket *socket) {
    for (auto &i : ServerCommon::getInstance()->getUserList()) {
        for (auto &j : i->socketInstances) {
            if (j == socket) return i;
        }
    }

    return nullptr;
}

static User *getUser(QString userName) {
    for (auto &i : ServerCommon::getInstance()->getUserList()) {
        if (i->getUserName() == userName) return i;
    }

    return nullptr;
}

void run(QTcpSocket *sender, QByteArray message) {
    message.replace("username ", "");
    QString username = message.mid(0, message.indexOf(" "));
    QHostAddress ip(sender->localAddress().toIPv4Address());

    if (getUser(username) == nullptr)
        ServerCommon::getInstance()->getUserList().append(new User(username, sender));
    else
        getUser(username)->socketInstances.append(sender);

    log()->Event(username + " (" + ip.toString() + ") connected.");
}