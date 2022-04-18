#ifndef ADDUSER_H
#define ADDUSER_H

#include <QObject>
#include <QtPlugin>

#include "../../lib/Logger/logger.h"
#include "../common/cmd_plugin_interface.h"
#include "../common/server_common.h"
#include "../common/user.h"

class CmdPlugin : public QObject, public CmdPluginInterface {
    Q_OBJECT
    Q_PLUGIN_METADATA(IID CmdPluginInterface_iid)
    Q_INTERFACES(CmdPluginInterface)

   public:
    ~CmdPlugin() override = default;
    void run(QTcpSocket *sender, [[maybe_unused]] QByteArray message) const override;

   private:
    static User *getUser(QTcpSocket *socket);
    static User *getUser(QString userName);
};
#endif  // ADDUSER_H
