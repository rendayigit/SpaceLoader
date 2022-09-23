#ifndef INTERNALCMD_H
#define INTERNALCMD_H

#include "BaseCmd.h"

class InternalCmd : public BaseCmd {
   public:
    InternalCmd(const InternalCmd &) = delete;
    InternalCmd &operator=(InternalCmd const &) = delete;
    InternalCmd(InternalCmd &&) = delete;
    InternalCmd &operator=(InternalCmd &&) = delete;
    ~InternalCmd() = default;

    static auto &getInstance() {
        static InternalCmd instance;
        return instance;
    }
    InternalCmd() = default;
};

#endif  // INTERNALCMD_H
