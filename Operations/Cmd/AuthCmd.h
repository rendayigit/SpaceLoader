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
};

#endif  // AUTHCMD_H
