#ifndef GETUSERLIST_H
#define GETUSERLIST_H

#include <QObject>
#include <QtPlugin>

#include "../common/cmd_plugin_interface.h"

class CmdPlugin : public QObject, public CmdPluginInterface {
    Q_OBJECT
    Q_PLUGIN_METADATA(IID CmdPluginInterface_iid)
    Q_INTERFACES(CmdPluginInterface)

   public:
    ~CmdPlugin() override = default;
    void run(QTcpSocket *sender, [[maybe_unused]] QByteArray message) const override;
};
#endif  // GETUSERLIST_H
