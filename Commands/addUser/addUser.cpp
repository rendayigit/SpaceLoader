#include "addUser.h"

User *TestPlugin::getUser(QTcpSocket *socket) {
    for (auto &i : ServerCommon::getInstance().getUserList()) {
        for (auto &j : i->socketInstances) {
            if (j == socket) return i;
        }
    }

    return nullptr;
}

User *TestPlugin::getUser(QString userName) {
    for (auto &i : ServerCommon::getInstance().getUserList()) {
        if (i->getUserName() == userName) return i;
    }

    return nullptr;
}

void TestPlugin::run(QTcpSocket *sender, QByteArray message) const {
    message.replace("username ", "");
    QString username = message.mid(0, message.indexOf(" "));
    QHostAddress ip(sender->localAddress().toIPv4Address());

    if (getUser(username) == nullptr)
        ServerCommon::getInstance().getUserList().append(new User(username, sender));
    else
        getUser(username)->socketInstances.append(sender);

    Log()->Event(username + " (" + ip.toString() + ") connected.");
}