#ifndef COMMON_H
#define COMMON_H

#include <QtCore/QByteArray>
#include <QtCore/QDateTime>
#include <QtCore/QDirIterator>
#include <QtCore/QLibrary>
#include <QtCore/QPluginLoader>
#include <QtCore/QString>
#include <QtCore/QtPlugin>
#include <QtNetwork/QNetworkInterface>
#include <QtNetwork/QTcpSocket>

#include "Commands/cmd_plugin_interface.h"
#include "lib/Logger/logger.h"
#include "path.h"

static QString GetCmd(QString msg) {
    msg = msg.simplified();
    return msg.mid(0, msg.indexOf(" ", 0));
}

// TODO add an index param to this function and get param from that index
static QString GetParam(QString msg) {
    msg = msg.simplified();
    return msg.mid(msg.indexOf(" ", 0) + 1, msg.size());
}

static bool Cmp(const QString source, const QString command) {
    return GetCmd(source).compare(command, Qt::CaseInsensitive) == 0;
}

static void Transmit(QTcpSocket *socket, QByteArray message, bool transmitTime = false) {
    if (socket != nullptr) {
        if (socket->state() != QAbstractSocket::SocketState::UnconnectedState) {
            if (transmitTime) {
                message = QDateTime::currentDateTime().toString().toLocal8Bit() + "\n" + message;
            }
            socket->write(message);
            socket->waitForBytesWritten();
            socket->flush();
        }
    }
}

// TODO this function may replace some lines of code in server.cpp
static QString GetIp(QTcpSocket *socket) {
    QHostAddress ip(socket->peerAddress().toIPv4Address());
    return ip.toString();
}

static QList<QString> GetLocalIp() {
    QList<QString> ipList;
    QHostAddress address;
    const QHostAddress &localhost = QHostAddress(QHostAddress::LocalHost);
    for (const QHostAddress &address : QNetworkInterface::allAddresses()) {
        if (address.protocol() == QAbstractSocket::IPv4Protocol && address != localhost) {
            ipList.append(address.toString());
        }
    }
    return ipList;
}

static QList<QString> getDlibs(QString path) {
    QList<QString> libList;

    QDirIterator iterator(path, QDirIterator::Subdirectories);

    while (iterator.hasNext()) {
        QFile file(iterator.next());
        if (file.open(QIODevice::ReadOnly) and QLibrary::isLibrary(file.fileName())) {
            libList.append(file.fileName());
        }
    }

    return libList;
}

static void runDynamicCmd(QTcpSocket *sender, QByteArray message) {
    QList<QString> commandLibs = getDlibs(Path::getCmdsDir());
    if (commandLibs.isEmpty()) {
        Log().Error("no libs found at " + Path::getCmdsDir());
    } else {
        for (auto &lib : commandLibs) {
            if (lib.contains(GetCmd(message), Qt::CaseInsensitive)) {
                QPluginLoader loader(lib);
                if (auto *instance = loader.instance()) {
                    if (auto *plugin = qobject_cast<CmdPluginInterface *>(instance)) {
                        plugin->run(sender, message);
                    } else {
                        Log().Error("qobject_cast<> returned nullptr");
                    }
                } else {
                    Log().Error(loader.errorString());
                }
            }
        }
    }
}

#endif  // COMMON_H