#include "Operations.h"

#include "../common.h"

Operations::Operations(QString yamlFile) {
    cmdsYamlFile = yamlFile;
    Log()->setFlushRate(0);
}

bool Operations::parseMessage(QTcpSocket *sender, QByteArray message) {
    for (auto &i : cmdList) {
        if (Cmp(message, i->getCmdCallString())) {
            switch (i->getCmdType()) {
            case CmdType::internal:
                parseInternalCmd(sender, message);
                break;
            case CmdType::call:
                runBatchScript(sender, dynamic_cast<CallCmd *>(i), message);
                break;
            case CmdType::fileTransfer:
                fileTransfer(sender, dynamic_cast<FileTransferCmd *>(i), message);
                break;
            case null:
            case trigger:
                break;
            }
            return true;
        }
    }

    return false;
}

bool Operations::checkTimerFlag(vector<YAML::Node> cmdsWithTheTimerFlagSetNodeList,
                                QString cmdCallString) {
    for (auto &i : cmdsWithTheTimerFlagSetNodeList) {
        if (Yaml::getValue(i, "CmdCallString") == cmdCallString.toStdString()) {
            return true;
        }
    }
    return false;
}

bool Operations::checkAuthFlag(vector<YAML::Node> cmdsWithTheAuthFlagSetNodeList,
                               QString cmdCallString) {
    for (auto &i : cmdsWithTheAuthFlagSetNodeList) {
        if (Yaml::getValue(i, "CmdCallString") == cmdCallString.toStdString()) {
            return true;
        }
    }
    return false;
}

void Operations::populateCmdLists() {
    cmdList.clear();

    auto cmdsWithTheTimerFlagSetNodeList =
        Yaml::getNodeListByKey(cmdsYamlFile.toStdString(), "timer", "true");
    auto cmdsWithTheAuthFlagSetNodeList =
        Yaml::getNodeListByKey(cmdsYamlFile.toStdString(), "auth", "true");
    auto callCmdsNodeList = Yaml::getNodeListByKey(cmdsYamlFile.toStdString(), "type", "call");
    auto fileTransferCmdsNodeList =
        Yaml::getNodeListByKey(cmdsYamlFile.toStdString(), "type", "filetransfer");
    auto triggerCmdsNodeList =
        Yaml::getNodeListByKey(cmdsYamlFile.toStdString(), "type", "trigger");
    auto internalCmdsNodeList =
        Yaml::getNodeListByKey(cmdsYamlFile.toStdString(), "type", "internal");

    for (auto &i : callCmdsNodeList) {
        auto *callCmd = new CallCmd();
        callCmd->setCmdId("");
        callCmd->setCmdType(CmdType::call);
        callCmd->setCmdCallString(QString::fromStdString(Yaml::getValue(i, "CmdCallString")));
        callCmd->setCmdDescription(QString::fromStdString(Yaml::getValue(i, "Description")));
        callCmd->setScriptDir(QString::fromStdString(Yaml::getValue(i, "ScriptDir")));
        callCmd->setScriptFileName(QString::fromStdString(Yaml::getValue(i, "id")));
        callCmd->setIsTimerSet(
            checkTimerFlag(cmdsWithTheTimerFlagSetNodeList, callCmd->getCmdCallString()));
        callCmd->setIsAuthRequired(
            checkAuthFlag(cmdsWithTheAuthFlagSetNodeList, callCmd->getCmdCallString()));

        if (callCmd->getIsTimerSet()) {
            callCmd->setTriggerTime(QString::fromStdString(Yaml::getValue(i, "type")));
        }

        cmdList.append(callCmd);
    }

    for (auto &i : fileTransferCmdsNodeList) {
        auto *fileTransferCmd = new FileTransferCmd();
        fileTransferCmd->setCmdId("");
        fileTransferCmd->setCmdType(CmdType::fileTransfer);
        fileTransferCmd->setCmdCallString(
            QString::fromStdString(Yaml::getValue(i, "CmdCallString")));
        fileTransferCmd->setCmdDescription(
            QString::fromStdString(Yaml::getValue(i, "Description")));
        fileTransferCmd->setDestinationDir(
            QString::fromStdString(Yaml::getValue(i, "DestinationDir")));
        fileTransferCmd->setIsTimerSet(false);
        fileTransferCmd->setIsAuthRequired(
            checkAuthFlag(cmdsWithTheAuthFlagSetNodeList, fileTransferCmd->getCmdCallString()));
        fileTransferCmd->setTriggerTime("00:00");

        cmdList.append(fileTransferCmd);
    }

    for (auto &i : internalCmdsNodeList) {
        auto *internalCmd = new InternalCmd();
        internalCmd->setCmdId("");
        internalCmd->setCmdType(CmdType::internal);
        internalCmd->setCmdCallString(QString::fromStdString(Yaml::getValue(i, "CmdCallString")));
        internalCmd->setIsTimerSet(
            checkTimerFlag(cmdsWithTheTimerFlagSetNodeList, internalCmd->getCmdCallString()));
        internalCmd->setIsAuthRequired(
            checkAuthFlag(cmdsWithTheAuthFlagSetNodeList, internalCmd->getCmdCallString()));
        internalCmd->setCmdDescription(QString::fromStdString(Yaml::getValue(i, "Description")));

        if (internalCmd->getIsTimerSet()) {
            internalCmd->setTriggerTime(QString::fromStdString(Yaml::getValue(i, "id")));
        }

        cmdList.append(internalCmd);
    }

    Log()->Info("Parsed ServerCmds.yaml");
}

void Operations::runBatchScript(QTcpSocket *sender, CallCmd *cmd, QString message) {
    QString batchLocation = cmd->getScriptDir();
    QString batchFile = cmd->getScriptFileName();

    QStringList args = message.split(" ");
    args.removeFirst();
    args.prepend(batchLocation + batchFile);
    args.prepend("/c");
    qDebug() << "running: " << args;

    auto *process = new QProcess();

    #ifdef Q_OS_WIN
        QString bash = "cmd.exe";
    #else
        QString bash = "/bin/bash";
    #endif
    process->start(bash, args);

    connectProcess(sender, process);
}

void Operations::timerTrigger() {
    QTime currentTime = QTime::currentTime();
    QTime currentHMTime = QTime(currentTime.hour(), currentTime.minute(), 0, 0);

    for (auto &cmd : cmdList) {
        if (cmd->getIsTimerSet() and cmd->getTriggerTime() == currentHMTime) {
            Log()->Info("Trigger by timer: " + cmd->getCmdCallString());
            parseMessage(nullptr, cmd->getCmdCallString().toLocal8Bit());
        }
    }
}

QString Operations::help() {
    QString helpString = "\n";

    for (auto &cmd : cmdList) {
        helpString += cmd->getCmdCallString() + ":\n" + cmd->getCmdDescription() + "\n\n";
    }

    return helpString;
}
