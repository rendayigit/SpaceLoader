#include <QDebug>
#include "getUserList.h"
#include <iostream>
#include "../common/serverCommon.h"
void TestPlugin::run(QTcpSocket *sender, QByteArray message) const 
{
    qDebug()<<"control";
    
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

    // TCPServer::transmit(sender, users.toLocal8Bit());//  symbol lookup error: undefined symbol: _ZN9TCPServer8transmitEP10QTcpSocket10QByteArray
}

