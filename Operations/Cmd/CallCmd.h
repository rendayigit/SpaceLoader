#ifndef CALLCMD_H
#define CALLCMD_H

#include "BaseCmd.h"

class CallCmd : public BaseCmd {
   public:
    CallCmd(const CallCmd &) = delete;
    ~CallCmd() override = default;

    CallCmd &operator=(CallCmd const &) = delete;
    CallCmd(CallCmd &&) = delete;
    CallCmd &operator=(CallCmd &&) = delete;
    CallCmd() = default;

    QString getScriptDir() const { return ScriptDir; }
    void setScriptDir(const QString &scriptDir) { ScriptDir = scriptDir; }

    QString getScriptFileName() const { return ScriptFileName; }
    void setScriptFileName(const QString &scriptFileName) { ScriptFileName = scriptFileName; }

   private:
    QString ScriptDir;
    QString ScriptFileName;
};

#endif  // CALLCMD_H
