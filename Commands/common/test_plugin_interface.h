#ifndef TEST_PLUGIN_INTERFACE
#define TEST_PLUGIN_INTERFACE

#include "../../lib/TCP/server/tcpServer.h"

class TestPluginInterface {
   public:
    virtual ~TestPluginInterface() = default;
    virtual void run(QTcpSocket *sender, [[maybe_unused]] QByteArray message) const = 0;
};

#define TestPluginInterface_iid "whatever.you.want.TestPluginInterface"
Q_DECLARE_INTERFACE(TestPluginInterface, TestPluginInterface_iid)
#endif  // TEST_PLUGIN_INTERFACE
