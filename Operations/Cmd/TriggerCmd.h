#ifndef TRIGGERCMD_H
#define TRIGGERCMD_H

#include "BaseCmd.h"

class TriggerCmd : public BaseCmd {
   public:
    TriggerCmd() = default;

    QString getTriggererId() const { return TriggererId; }
    void setTriggererId(const QString &triggererId) { TriggererId = triggererId; }

    QString getCallString() const { return CallString; }
    void setCallString(const QString &callString) { CallString = callString; }

   private:
    QString TriggererId;
    QString CallString;
};

#endif  // TRIGGERCMD_H
