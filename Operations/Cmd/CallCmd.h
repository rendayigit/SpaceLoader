#ifndef CALLCMD_H
#define CALLCMD_H

#include "BaseCmd.h"

class CallCmd : public BaseCmd {
   public:
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
