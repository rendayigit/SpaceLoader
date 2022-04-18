#ifndef TEST_PLUGIN_INTERFACE
#define TEST_PLUGIN_INTERFACE

#include "../../lib/TCP/server/tcpServer.h"

class CmdPluginInterface {
   public:
    virtual ~CmdPluginInterface() = default;
    virtual void run(QTcpSocket *sender, [[maybe_unused]] QByteArray message) const = 0;
};

#define CmdPluginInterface_iid "SpaceLoader.CmdPluginInterface"
Q_DECLARE_INTERFACE(CmdPluginInterface, CmdPluginInterface_iid)
#endif  // TEST_PLUGIN_INTERFACE
