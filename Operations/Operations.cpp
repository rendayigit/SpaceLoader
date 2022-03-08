#include "Operations.h"


Operations::Operations(QString yamlFile) {
    cmdsYamlFile = yamlFile;
    log()->FLUSHRATE = 0;
}

bool Operations::parseMessage(QTcpSocket *sender, QByteArray message, bool isExactMatch) {
    for (auto &i : cmdList) {
        if (cmp(message, i->getCmdCallString())) {
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

            // Check for triggers connected to command
            for (auto &tcmd : cmdList) {
                if (tcmd->getCmdType() == CmdType::trigger) {
                    auto *trigger = dynamic_cast<TriggerCmd *>(tcmd);
                    if (cmp(i->getCmdCallString(), trigger->getTriggererId())) {
                        QString triggerCmd = trigger->getCallString();
                        log()->Info("Detected a trigger connected to " + i->getCmdCallString());
                        log()->Info("Triggering " + triggerCmd);
                        parseMessage(sender, triggerCmd.toLocal8Bit());
                        return true;
                    }
                }
            }
            return true;
        }
    }

    return false;
}

QString Operations::helpFormatter(QString message) {
    for (int i = 80; i < message.size(); i += 81) {
        message.insert(i, "\n");
        for (int j = 0; j < 34; j++) {
            message.insert(i + 1, " ");
        }
    }
    return message;
}

bool Operations::checkTimerFlag(vector<YAML::Node> cmdsWithTheTimerFlagSetNodeList,
                                QString cmdCallString) {
    for (auto &i : cmdsWithTheTimerFlagSetNodeList) {
        if (Yaml::getText<string>(i, "CmdCallString") == cmdCallString.toStdString()) {
            return true;
        }
    }
    return false;
}

bool Operations::checkAuthFlag(vector<YAML::Node> cmdsWithTheAuthFlagSetNodeList,
                               QString cmdCallString) {
    for (auto &i : cmdsWithTheAuthFlagSetNodeList) {
        if (Yaml::getText<string>(i, "CmdCallString") == cmdCallString.toStdString()) {
            return true;
        }
    }
    return false;
}

void Operations::populateCmdLists() {
    cmdList.clear();

    auto cmdsWithTheTimerFlagSetNodeList =
        Yaml::getNodeListByTag<vector<YAML::Node>>(cmdsYamlFile.toStdString(), "timer", "true");
    auto cmdsWithTheAuthFlagSetNodeList =
        Yaml::getNodeListByTag<vector<YAML::Node>>(cmdsYamlFile.toStdString(), "auth", "true");
    auto callCmdsNodeList =
        Yaml::getNodeListByTag<vector<YAML::Node>>(cmdsYamlFile.toStdString(), "type", "call");
    auto fileTransferCmdsNodeList =
        Yaml::getNodeListByTag<vector<YAML::Node>>(cmdsYamlFile.toStdString(), "type", "filetransfer");
    auto triggerCmdsNodeList =
        Yaml::getNodeListByTag<vector<YAML::Node>>(cmdsYamlFile.toStdString(), "type", "trigger");
    auto internalCmdsNodeList =
        Yaml::getNodeListByTag<vector<YAML::Node>>(cmdsYamlFile.toStdString(), "type", "internal");

    for (auto &i : callCmdsNodeList) {
        auto *callCmd = new CallCmd();
        callCmd->setCmdId("");
        callCmd->setCmdType(CmdType::call);
        callCmd->setCmdCallString(QString::fromStdString(Yaml::getText<string>(i, "CmdCallString")));
        callCmd->setCmdDescription(QString::fromStdString(Yaml::getText<string>(i, "Description")));
        callCmd->setScriptDir(QString::fromStdString(Yaml::getText<string>(i, "ScriptDir")));
        callCmd->setScriptFileName(QString::fromStdString(Yaml::getText<string>(i, "id")));
        callCmd->setIsTimerSet(
            checkTimerFlag(cmdsWithTheTimerFlagSetNodeList, callCmd->getCmdCallString()));
        callCmd->setIsAuthRequired(
            checkAuthFlag(cmdsWithTheAuthFlagSetNodeList, callCmd->getCmdCallString()));

        if (callCmd->getIsTimerSet()) {
            callCmd->setTriggerTime(QString::fromStdString(Yaml::getText<string>(i, "type")));
        }

        cmdList.append(callCmd);
    }

    for (auto &i : fileTransferCmdsNodeList) {
        auto *fileTransferCmd = new FileTransferCmd();
        fileTransferCmd->setCmdId("");
        fileTransferCmd->setCmdType(CmdType::fileTransfer);
        fileTransferCmd->setCmdCallString(QString::fromStdString(Yaml::getText<string>(i, "CmdCallString")));
        fileTransferCmd->setCmdDescription(QString::fromStdString(Yaml::getText<string>(i, "Description")));
        fileTransferCmd->setDestinationDir(QString::fromStdString(Yaml::getText<string>(i, "DestinationDir")));
        fileTransferCmd->setIsTimerSet(false);
        fileTransferCmd->setIsAuthRequired(
            checkAuthFlag(cmdsWithTheAuthFlagSetNodeList, fileTransferCmd->getCmdCallString()));
        fileTransferCmd->setTriggerTime("00:00");

        cmdList.append(fileTransferCmd);
    }

    for (auto &i : triggerCmdsNodeList) {
        auto *triggerCmd = new TriggerCmd();
        triggerCmd->setCmdId("");
        triggerCmd->setCmdType(CmdType::trigger);
        triggerCmd->setCmdCallString(QString::fromStdString(Yaml::getText<string>(i, "CmdCallString")));
        triggerCmd->setCmdDescription(QString::fromStdString(Yaml::getText<string>(i, "Description")));
        triggerCmd->setTriggererId(QString::fromStdString(Yaml::getText<string>(i, "id")));
        triggerCmd->setCallString(QString::fromStdString(Yaml::getText<string>(i, "type")));
        triggerCmd->setIsTimerSet(false);
        triggerCmd->setIsAuthRequired(
            checkAuthFlag(cmdsWithTheAuthFlagSetNodeList, triggerCmd->getCmdCallString()));
        triggerCmd->setTriggerTime("00:00");

        cmdList.append(triggerCmd);
    }

    for (auto &i : internalCmdsNodeList) {
        auto *internalCmd = new InternalCmd();
        internalCmd->setCmdId("");
        internalCmd->setCmdType(CmdType::internal);
        internalCmd->setCmdCallString(QString::fromStdString(Yaml::getText<string>(i, "CmdCallString")));
        internalCmd->setIsTimerSet(
            checkTimerFlag(cmdsWithTheTimerFlagSetNodeList, internalCmd->getCmdCallString()));
        internalCmd->setIsAuthRequired(
            checkAuthFlag(cmdsWithTheAuthFlagSetNodeList, internalCmd->getCmdCallString()));
        internalCmd->setCmdDescription(QString::fromStdString(Yaml::getText<string>(i, "Description")));

        if (internalCmd->getIsTimerSet()) {
            internalCmd->setTriggerTime(QString::fromStdString(Yaml::getText<string>(i, "id")));
        }

        cmdList.append(internalCmd);
    }

    log()->Info("Parsed ServerCmds.yaml");
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
    process->start("cmd.exe", args);

    connectProcess(sender, process);
}

void Operations::transmitMsg(QTcpSocket *client, QByteArray message) {
    if (client->state() != QAbstractSocket::SocketState::UnconnectedState) {
        client->write(message);
        client->waitForBytesWritten();
    }
}

// TODO move to Commands/common
bool Operations::cmp(const QString source, const QString command) {
    QString simplifiedSource = source.simplified();
    simplifiedSource = simplifiedSource.mid(0, simplifiedSource.indexOf(" ", 0));

    return simplifiedSource.compare(command, Qt::CaseInsensitive) == 0;
}

void Operations::timerTrigger() {
    QTime currentTime = QTime::currentTime();
    QTime currentHMTime = QTime(currentTime.hour(), currentTime.minute(), 0, 0);

    for (auto &cmd : cmdList) {
        if (cmd->getIsTimerSet() and cmd->getTriggerTime() == currentHMTime) {
            log()->Info("Trigger by timer: " + cmd->getCmdCallString());
            parseMessage(nullptr, cmd->getCmdCallString().toLocal8Bit());
        }
    }
}