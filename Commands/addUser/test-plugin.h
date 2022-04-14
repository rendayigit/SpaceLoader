#ifndef ADDUSER_H
#define ADDUSER_H

#include <QtCore/QObject>
#include <QtCore/QtPlugin>

#include "../../lib/Logger/logger.h"
#include "../common/test-plugin-interface.h"
#include "../common/serverCommon.h"
#include "../common/user.h"

class TestPlugin : public QObject, public TestPluginInterface {
    Q_OBJECT
    Q_PLUGIN_METADATA(IID TestPluginInterface_iid)
    Q_INTERFACES(TestPluginInterface)

   public:
    ~TestPlugin() override {}
    void run(QTcpSocket *sender, [[maybe_unused]] QByteArray message) const override;

   private:
    static User *getUser(QTcpSocket *socket);
    static User *getUser(QString userName);
};
#endif  // ADDUSER_H
