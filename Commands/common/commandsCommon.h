#include <QtNetwork/qtcpsocket.h>
#include <QtCore/QtCore>
#include <QTcpSocket>
#include <QString>
#include <QByteArray>
#include <QDateTime>

static bool cmp(const QString source, const QString command) {
    QString simplifiedSource = source.simplified();
    simplifiedSource = simplifiedSource.mid(0, simplifiedSource.indexOf(" ", 0));

    return simplifiedSource.compare(command, Qt::CaseInsensitive) == 0;
}

static void transmit(QTcpSocket *socket, QByteArray message) {
    if (socket != nullptr) {
        if (socket->state() != QAbstractSocket::SocketState::UnconnectedState) {
            socket->write(QDateTime::currentDateTime().toString().toLocal8Bit() + "\n" + message);
            socket->waitForBytesWritten();
        }
    }
}