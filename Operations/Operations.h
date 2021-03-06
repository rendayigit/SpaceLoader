#ifndef Operations_H
#define Operations_H

#include <QtCore/QByteArray>
#include <QtCore/QCoreApplication>
#include <QtCore/QDebug>
#include <QtCore/QFile>
#include <QtCore/QPair>
#include <QtCore/QProcess>
#include <QtCore/QTime>
#include <QtNetwork/QHostAddress>
#include <QtNetwork/QTcpSocket>
#include "../lib/YAML/yaml.h"
#include "../common.h"
#include "../lib/Logger/logger.h"
#include "./Cmd/AuthCmd.h"
#include "./Cmd/CallCmd.h"
#include "./Cmd/FileTransferCmd.h"
#include "./Cmd/InternalCmd.h"
#include "./Cmd/TriggerCmd.h"

using namespace std;

class Operations {
   public:
    Operations(QString cmdsYamlFile);

    struct parsedCmd {
        CmdType cmdType;
        int listIndex;
        QString originalMessage;
    };

    bool parseMessage(QTcpSocket *sender, QByteArray message, bool isExactMatch = false);
    void populateCmdLists();
    QString helpFormatter(QString message);

    parsedCmd getParsedMessage(QString cmd);
    static bool cmp(const QString source, const QString command);

    void runBatchScript(QTcpSocket *sender, CallCmd *cmd, QString message);
    bool isAuthorized(QTcpSocket *sender, QString cmdName);

    virtual void fileTransfer(QTcpSocket *sender, FileTransferCmd *cmd, QByteArray message) = 0;
    virtual void parseInternalCmd(QTcpSocket *sender, QByteArray message) = 0;
    virtual void connectProcess(QTcpSocket *sender, QProcess *process) = 0;

    void timerTrigger();
    void transmitMsg(QTcpSocket *client, QByteArray message);
    

   private:
    static bool checkTimerFlag(std::vector<YAML::Node> cmdsWithTheTimerFlagSetNodeList,
                               QString cmdCallString);
    static bool checkAuthFlag(std::vector<YAML::Node> cmdsWithTheAuthFlagSetNodeList,
                              QString cmdCallString);

    QString cmdsYamlFile;

   protected:
    QList<BaseCmd *> cmdList;
};

#endif  // Operations_H
