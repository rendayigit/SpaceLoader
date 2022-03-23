#ifndef GETUSERLIST_H
#define GETUSERLIST_H

#include <QtCore/QObject>
#include <QtCore/QtPlugin>

#include "../../PluginInterface.h"
#include "../common/serverCommon.h"

class GetUserList : public QObject, public PluginInterface {
    Q_OBJECT
    Q_PLUGIN_METADATA(IID PluginInterface_iid)
    Q_INTERFACES(PluginInterface)

   public:
    ~GetUserList() override {}
    void run(QTcpSocket *sender, [[maybe_unused]] QByteArray message) const override;
};
#endif  // GETUSERLIST_H
