#ifndef BASECMD_H
#define BASECMD_H

#include <QtCore/QtCore>

enum CmdType { null, call, fileTransfer, trigger, internal };

class BaseCmd {
   public:
    BaseCmd() = default;
    virtual ~BaseCmd() = default;
    
    BaseCmd(const BaseCmd &) = delete;
    BaseCmd &operator=(BaseCmd const &) = delete;
    BaseCmd(BaseCmd &&) = delete;
    BaseCmd &operator=(BaseCmd &&) = delete;

    QString getCmdCallString() const { return CmdCallString; }
    void setCmdCallString(const QString &cmdCallString) { CmdCallString = cmdCallString; }

    QString getCmdId() const { return cmdId; }
    void setCmdId(const QString &cmdId_) { cmdId = cmdId_; }

    CmdType getCmdType() const { return cmdType; }
    void setCmdType(const CmdType &cmdType_) { cmdType = cmdType_; }

    bool getIsTimerSet() const { return isTimerSet; }
    void setIsTimerSet(bool isTimerSet_) { isTimerSet = isTimerSet_; }

    QTime getTriggerTime() const {
        int h = triggerTime.mid(0, 2).toUInt();
        int m = triggerTime.mid(3, 2).toUInt();

        return {h, m, 0, 0};
    }
    void setTriggerTime(const QString &triggerTime_) { triggerTime = triggerTime_; }

    QString getCmdDescription() const { return cmdDescription; }
    void setCmdDescription(const QString &cmdDescription_) { cmdDescription = cmdDescription_; }

    bool getIsAuthRequired() const { return authRequired; }
    void setIsAuthRequired(const bool isAuthRequired) { authRequired = isAuthRequired; }

    QString getAuthorizedUser() const { return authorizedUser; }

    QString authorizeUser(const QString &authorizedUser_) {
        if (authorizedUser.isEmpty()) {
            authorizedUser = authorizedUser_;
            return "You are now authorized for " + CmdCallString;
        }

        if (authorizedUser == authorizedUser_) {
            return "You are already authorized for " + CmdCallString;
        }

        return "Unable to acquire authorization";
    }

    QString forceAuthorizeUser(const QString &authorizedUser_) {
        authorizedUser = authorizedUser_;
        return "You are now authorized for " + CmdCallString;
    }

    QString clearAuthorizedUser() {
        authorizedUser.clear();
        return "Cleared authorization for " + CmdCallString;
    }

    bool isAuthenticated(QString userName) {
        return not authRequired or authorizedUser == userName;
    }

   private:
    QString CmdCallString;
    QString cmdDescription;
    QString cmdId;
    CmdType cmdType;
    bool isTimerSet;
    bool authRequired;
    QString triggerTime;
    QString authorizedUser;
};

#endif  // BASECMD_H
