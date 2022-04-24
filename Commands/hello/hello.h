#ifndef HELLO_H
#define HELLO_H

#include <QtCore/QObject>
#include <QtCore/QtPlugin>

#include "../cmd_plugin_interface.h"

class CmdPlugin : public QObject, public CmdPluginInterface {
    Q_OBJECT
    Q_PLUGIN_METADATA(IID CmdPluginInterface_iid)
    Q_INTERFACES(CmdPluginInterface)

   public:
    CmdPlugin() { description = "This is a sample plugin"; }
    ~CmdPlugin() override = default;
    void run(QTcpSocket *sender, [[maybe_unused]] QByteArray message) const override;
};
#endif  // HELLO_H
