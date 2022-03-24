#ifndef ADDUSER_H
#define ADDUSER_H

#include <QtCore/QObject>
#include <QtCore/QtPlugin>

#include "../lib/Logger/logger.h"
#include "PluginInterface.h"
#include "common/serverCommon.h"
#include "common/user.h"

class AddUser : public QObject, public PluginInterface {
    Q_OBJECT
    Q_PLUGIN_METADATA(IID PluginInterface_iid)
    Q_INTERFACES(PluginInterface)

   public:
    ~AddUser() override {}
    void run(QTcpSocket *sender, [[maybe_unused]] QByteArray message) const override;

   private:
    static User *getUser(QTcpSocket *socket);
    static User *getUser(QString userName);
};
#endif  // ADDUSER_H
