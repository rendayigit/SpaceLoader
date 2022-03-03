#ifndef USER_H
#define USER_H

#include <QtCore/QtCore>
#include <QtNetwork/QTcpSocket>

class User {
   public:
    User(QString userName, QTcpSocket *socket) {
        this->userName = userName;
        socketInstances.append(socket);
    };

    QString getUserName() const { return userName; }
    void setUsername(const QString &userName_) { userName = userName_; }

    QList<QTcpSocket *> socketInstances;

   private:
    QString userName = "";
};

#endif  // USER_H
