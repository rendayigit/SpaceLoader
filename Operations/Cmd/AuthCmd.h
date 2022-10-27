#ifndef AUTHCMD_H
#define AUTHCMD_H

#include "BaseCmd.h"

class AuthCmd : public BaseCmd {
   public:
    AuthCmd() = default;
    ~AuthCmd() override = default;
    
    AuthCmd(const AuthCmd &) = delete;
    AuthCmd &operator=(AuthCmd const &) = delete;
    AuthCmd(AuthCmd &&) = delete;
    AuthCmd &operator=(AuthCmd &&) = delete;
};

#endif  // AUTHCMD_H
