#ifndef INTERNALCMD_H
#define INTERNALCMD_H

#include "BaseCmd.h"

class InternalCmd : public BaseCmd {
   public:
    InternalCmd() = default;
    ~InternalCmd() override = default;

    InternalCmd(const InternalCmd &) = delete;
    InternalCmd &operator=(InternalCmd const &) = delete;
    InternalCmd(InternalCmd &&) = delete;
    InternalCmd &operator=(InternalCmd &&) = delete;
};

#endif  // INTERNALCMD_H
