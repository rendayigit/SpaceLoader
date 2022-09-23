#ifndef AUTHCMD_H
#define AUTHCMD_H

#include "BaseCmd.h"

class AuthCmd : public BaseCmd {
   public:
    AuthCmd(const AuthCmd &) = delete;
    AuthCmd &operator=(AuthCmd const &) = delete;
    AuthCmd(AuthCmd &&) = delete;
    AuthCmd &operator=(AuthCmd &&) = delete;
    AuthCmd() = default;

    static auto &getInstance() {
        static AuthCmd instance;
        return instance;
    }
};

#endif  // AUTHCMD_H
