#include "server.h"

#include "../Commands/cmd_plugin_interface.h"
#include "User/userOperations.h"
#include "logging.h"

Server::Server() : Operations(Paths().getServerCmdsYaml()) {
    isFileTransferInProgress = false;

    triggerCmdTimer = new QTimer(this);
    triggerCmdTimer->setInterval(SERVER_TRIGGER_COMMAND_TIMER_INTERVAL);
    connect(triggerCmdTimer, &QTimer::timeout, this, [=]() { Operations::timerTrigger(); });

    QMetaObject::invokeMethod(
        this, [=]() -> void { triggerCmdTimer->start(); }, Qt::QueuedConnection);

    Log().Info("Server Started");
}

void Server::onReceived(QTcpSocket *sender, QByteArray message) {
    QHostAddress ip4Address(sender->peerAddress().toIPv4Address());
    QString cmdName;

    if (message.indexOf(" ") != -1) {
        cmdName = message.mid(0, message.indexOf(" "));
    } else {
        cmdName = message;
    }

    if (not isFileTransferInProgress) {  // No file transfer operations, regular operations
        Log().Info("Received: '" + message + "' from " + ip4Address.toString());

        if (getCmd(cmdName) != nullptr and getCmd(cmdName)->getIsAuthRequired() and
            not getCmd(GetParam(message))
                    ->isAuthenticated(
                        UserOperations::getInstance().getUser(sender)->getUserName())) {
            transmit(sender, "You are not authorized for this command. Try: getAuth " +
                                 cmdName.toLocal8Bit());
            return;
        }

        if (not parseMessage(sender, message)) {
            transmit(sender, "I don't understand that command :(");
        }

        std::cout << message.toStdString() << std::endl;
    } else {  // File transfer operations
        if (sender == fileTransfererSocket) {
            if (not Cmp(message.data(), "#END")) {
                transferredFileBuffer.append(message);
                formerCurrTime = QDateTime::currentDateTime();
            } else {  // Finalize file transfer
                isFileTransferInProgress = false;
                QFile file(transferredFileLocation + transferredFileName);
                qInfo() << "Copied file to " << transferredFileLocation + transferredFileName;
                file.open(QIODevice::WriteOnly);
                file.write(transferredFileBuffer);
                file.close();
                transferredFileBuffer.clear();
            }
        } else {
            if (formerCurrTime.msecsTo(QDateTime::currentDateTime()) > 5000) {
                isFileTransferInProgress = false;
                transferredFileBuffer.clear();
                Log().Info("File transfer timed out");
                onReceived(sender, message);
            } else {
                transmit(sender,
                         "File transfer to server in progress. Please try again "
                         "later..");
            }
        }
    }
}

void Server::clientDisconnected(QTcpSocket *clientSocket) {
    User *user = UserOperations::getInstance().getUser(clientSocket);

    clearUserAuths(clientSocket);

    UserOperations::getInstance().removeUser(clientSocket);
}

void Server::fileTransfer(QTcpSocket *sender, QString localFile, QString serverPath) {
    fileTransfererSocket = sender;
    isFileTransferInProgress = true;

    transferredFileName = localFile.mid(localFile.lastIndexOf("/") + 1, localFile.length());
    transferredFileLocation = serverPath;
}

BaseCmd *Server::getCmd(QString cmdName) {
    for (auto &i : cmdList) {
        if (Cmp(cmdName, i->getCmdCallString())) {
            return i;
        }
    }

    return nullptr;
}

void Server::parseInternalCmd(QTcpSocket *sender, QByteArray message) {
    if (Cmp(message, "addUser")) {
        UserOperations::getInstance().addUser(sender, message);
        broadcast(UserOperations::getInstance().getUserList(sender).toLocal8Bit());
    } else if (Cmp(message, "getUserList")) {
        Transmit(sender, UserOperations::getInstance().getUserList(sender).toLocal8Bit());
    } else if (Cmp(message, "listLogs")) {
        Transmit(sender, Logging::getLogFileNames().toLocal8Bit());
    } else if (Cmp(message, "readLog")) {
        Transmit(sender, Logging::getLogData(GetParam(message)).toLocal8Bit());
    } else if (Cmp(message, "help")) {
        Transmit(sender, (" - Server Commands - \n" + Operations::help()).toLocal8Bit());
    } else if (Cmp(message, "sa")) {
        Transmit(sender, "AS");
    } else if (Cmp(message, "UpdateCmds")) {
        populateCmdLists();
        Transmit(sender, "Command list updated.");
    } else if (Cmp(message, "getAuth")) {
        Transmit(sender,
                 getCmd(GetParam(message))
                     ->authorizeUser(UserOperations::getInstance().getUser(sender)->getUserName())
                     .toLocal8Bit());
    } else if (Cmp(message, "clearAuth")) {
        if (not message.simplified().contains(" ")) {
            clearUserAuths(sender);
        } else {
            if (getCmd(GetParam(message))
                    ->isAuthenticated(
                        UserOperations::getInstance().getUser(sender)->getUserName())) {
                Transmit(sender, getCmd(GetParam(message))->clearAuthorizedUser().toLocal8Bit());
            } else {
                Transmit(sender, "You are already not authorized for this command.");
            }
        }
    } else if (Cmp(message, "forceAuth")) {
        Transmit(sender, getCmd(GetParam(message))
                             ->forceAuthorizeUser(
                                 UserOperations::getInstance().getUser(sender)->getUserName())
                             .toLocal8Bit());
    } else if (Cmp(message, "transmit")) {
        message = message.simplified();

        int sIndex = message.indexOf("-s", 0) + 3;
        int dIndex = message.indexOf("-d") - 1;

        QString localFileAndPath = message.mid(sIndex, dIndex - sIndex).simplified();
        QString serverPath = message.mid(dIndex + 4, message.length()).simplified();

        std::cout << "localFileAndPath: " << localFileAndPath.toStdString() << std::endl;
        std::cout << "serverPath: " << serverPath.toStdString() << std::endl;

        fileTransfer(sender, localFileAndPath, serverPath);
    } else if (Cmp(message, "plugin")) {
        QPluginLoader loader(Path::getInstance().getExecutablePath() + "cmds/lib" +
                             GetParam(message).mid(0, GetParam(message).indexOf(" ")) + ".dll");
        std::cout << (Path::getInstance().getExecutablePath() + "cmds/lib" +
                      GetParam(message).mid(0, GetParam(message).indexOf(" ")) + ".dll")
                         .toStdString()
                  << std::endl;
        if (auto *instance = loader.instance()) {
            if (auto *plugin = qobject_cast<CmdPluginInterface *>(instance)) {
                plugin->run(sender, GetParam(message).toLocal8Bit());
            } else {
                Log().Error("qobject_cast<> returned nullptr");
            }
        } else {
            Log().Error(loader.errorString());
        }
    }
}

void Server::clearUserAuths(QTcpSocket *sender) {
    for (auto &cmd : cmdList) {
        if (cmd->getAuthorizedUser() ==
            UserOperations::getInstance().getUser(sender)->getUserName()) {
            Transmit(sender, cmd->clearAuthorizedUser().toLocal8Bit());
        }
    }

    Transmit(sender, "All authorizations cleared.");
}

void Server::connectProcess(QTcpSocket *sender, QProcess *process) {
    Transmit(sender, "Script execution successful. Forwarding output: ");
    connect(process, &QProcess::readyReadStandardOutput, this,
            [=]() { Transmit(sender, process->readLine()); });
}