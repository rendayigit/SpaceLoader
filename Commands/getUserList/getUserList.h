#ifndef TEST_PLUGIN_HPP
#define TEST_PLUGIN_HPP

#include <QObject>
#include <QtPlugin>

#include "../common/test_plugin_interface.h"

class TestPlugin : public QObject, public TestPluginInterface {
    Q_OBJECT
    Q_PLUGIN_METADATA(IID TestPluginInterface_iid)
    Q_INTERFACES(TestPluginInterface)

   public:
    ~TestPlugin() override = default;
    void run(QTcpSocket *sender, [[maybe_unused]] QByteArray message) const;
};
#endif  // TEST_PLUGIN_HPP
