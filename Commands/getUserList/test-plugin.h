#ifndef TEST_PLUGIN_HPP
#define TEST_PLUGIN_HPP
#include <QObject>
#include <QtPlugin>
#include "../common/test-plugin-interface.h"

class TestPlugin : public QObject, public TestPluginInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID TestPluginInterface_iid)
    Q_INTERFACES(TestPluginInterface)
    QString str;

public:
    ~TestPlugin() override {}
     void run(QTcpSocket *sender, [[maybe_unused]] QByteArray message) const ;
};
#endif // TEST_PLUGIN_HPP

