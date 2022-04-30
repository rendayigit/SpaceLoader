#include "server.h"

#include "User/userOperations.h"
#include "logging.h"

Server::Server() : Operations(Paths().getServerCmdsYaml()) {
    isFileTransferInProgress = false;

    triggerCmdTimer = new QTimer(this);
    triggerCmdTimer->setInterval(SERVER_TRIGGER_COMMAND_TIMER_INTERVAL);
    connect(triggerCmdTimer, &QTimer::timeout, this, [=]() { Operations::timerTrigger(); });

    QMetaObject::invokeMethod(
        this, [=]() -> void { triggerCmdTimer->start(); }, Qt::QueuedConnection);

    Log()->Info("Server Started");
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
        Log()->Info("Received: '" + message + "' from " + ip4Address.toString());

        if (getCmd(cmdName) != nullptr and getCmd(cmdName)->getIsAuthRequired() and
            not getCmd(GetParam(message))->isAuthenticated(UserOperations::getInstance().getUser(sender)->getUserName())) {
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
                Log()->Info("File transfer timed out");
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

    UserOperations::getInstance().removeUser(user);
    Log()->Info(user->getIp() + " disconnected");
}

void Server::fileTransfer(QTcpSocket *sender, FileTransferCmd *cmd, QByteArray message) {
    fileTransfererSocket = sender;
    isFileTransferInProgress = true;

    message = message.replace('\\', '/');

    int beginningOfFileName = message.lastIndexOf('/');
    transferredFileName = message.mid(beginningOfFileName + 1, message.size());

    transferredFileLocation = cmd->getDestinationDir();
}

BaseCmd *Server::getCmd(QString cmdName) {
    for (auto &i : cmdList) {
        if (Cmp(cmdName, i->getCmdCallString())) return i;
    }

    return nullptr;
}

void Server::parseInternalCmd(QTcpSocket *sender, QByteArray message) {
    if (Cmp(message, "addUser")) {
        UserOperations::getInstance().addUser(sender, message);
    } else if (Cmp(message, "getUserList")) {
        UserOperations::getInstance().getUserList(sender);
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