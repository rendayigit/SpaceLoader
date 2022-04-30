#include "user.h"

#include <QtNetwork/QHostAddress>

void User::addSocket(QTcpSocket *socket) { socketInstances.append(socket); }

QString User::getIp() {
    // TODO this function may replace some lines of code in server.cpp
    QHostAddress ip(socketInstances.at(0)->localAddress().toIPv4Address());
    return ip.toString();
}
