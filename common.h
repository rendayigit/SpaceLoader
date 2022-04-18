#ifndef COMMON_H
#define COMMON_H

#include <QtCore/QByteArray>
#include <QtCore/QDateTime>
#include <QtCore/QString>
#include <QtNetwork/QTcpSocket>

static QString GetCmd(QString msg) {
    msg = msg.simplified();
    return msg.mid(0, msg.indexOf(" ", 0));
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
        }
    }
}

#endif  // COMMON_H