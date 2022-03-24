#ifndef PLUGIN_INTERFACE
#define PLUGIN_INTERFACE

#include <QtCore/QtPlugin>

#include "../lib/TCP/server/tcpServer.h"

class PluginInterface {
   public:
    virtual ~PluginInterface() {}
    virtual void run(QTcpSocket *sender, [[maybe_unused]] QByteArray message) const = 0;
};

#define PluginInterface_iid "SpaceLoader.Command.PluginInterface"
Q_DECLARE_INTERFACE(PluginInterface, PluginInterface_iid)

#endif  // PLUGIN_INTERFACE
