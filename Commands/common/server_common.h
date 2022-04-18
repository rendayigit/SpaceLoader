#ifndef SERVER_COMMON_H
#define SERVER_COMMON_H

#include <QList>

#include "cmd_common.h"
#include "user.h"

class ServerCommon {
   public:
    static auto &getInstance() {
        static ServerCommon instance;
        return instance;
    }

    QList<User *> getUserList() { return userList; }

   private:
    ServerCommon() = default;
    QList<User *> userList;
};

#endif  // SERVER_COMMON_H