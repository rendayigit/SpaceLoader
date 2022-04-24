#include "user.h"

#include <QtNetwork/QHostAddress>

void User::addSocket(QTcpSocket *socket) { socketInstances.append(socket); }

QString User::getIp() {
    // TODO this function may replace some lines of code in server.cpp
    QHostAddress ip(socketInstances.at(0)->localAddress().toIPv4Address());
    return ip.toString();
}

bool User::checkAuthorization(BaseCmd *cmd) {
    for (auto &authorizedCmd : authorizedCmds) {
        if (authorizedCmd->getCmdCallString() == cmd->getCmdCallString()) {
            return true;
        }
    }

    return false;
}

bool User::addAuthorizedCmd(BaseCmd *cmd) {
    if (authorizedCmds.contains(cmd)) {
        return true;
    }

    if (cmd->getAuthorizedUser() == nullptr and cmd->getIsAuthRequired()) {
        authorizedCmds.append(cmd);
        cmd->setAuthorizedUser(userName);
        return true;
    }

    return false;
}

void User::removeAuthorizedCmd(BaseCmd *cmd) {
    if (authorizedCmds.contains(cmd)) {
        authorizedCmds.removeOne(cmd);
    }
}

void User::clearAuthorizations() {
    for (auto &cmd : authorizedCmds) {
        cmd->setAuthorizedUser(nullptr);
    }
    authorizedCmds.clear();
}
