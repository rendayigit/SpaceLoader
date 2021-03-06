#include "GetUserList.h"

void GetUserList::run(QTcpSocket *sender, [[maybe_unused]] QByteArray message) const {
    QString users;

    for (int i = 0; i < ServerCommon::getInstance()->getUserList().size(); i++) {
        QHostAddress ip(ServerCommon::getInstance()
                            ->getUserList()
                            .at(i)
                            ->socketInstances.at(0)
                            ->localAddress()
                            .toIPv4Address());
        users += "User #" + QString::number(i) +
                 " username: " + ServerCommon::getInstance()->getUserList().at(i)->getUserName() +
                 '\n';
        users += "User #" + QString::number(i) + " ip: " + ip.toString() + '\n';
    }

    TCPServer::transmit(sender, users.toLocal8Bit());
}