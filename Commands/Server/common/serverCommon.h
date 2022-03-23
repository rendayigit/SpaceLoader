#ifndef SERVERCOMMON_H
#define SERVERCOMMON_H

#include <QtCore/QList>

#include "../../commandsCommon.h"
#include "user.h"

class ServerCommon {
   public:
    static ServerCommon *getInstance() {
        if (m_instance == nullptr) m_instance = new ServerCommon;
        return m_instance;
    }
    QList<User *> getUserList() { return userList; }

   private:
    ServerCommon() {}
    static ServerCommon *m_instance;
    QList<User *> userList;
};

ServerCommon *ServerCommon::m_instance = ServerCommon::getInstance();

#endif  // SERVERCOMMON_H