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

        if (getCmd(cmdName) != nullptr && getCmd(cmdName)->getIsAuthRequired() &&
            not isAuthorized(sender, cmdName)) {
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
    if (UserOperations::getInstance().getUser(clientSocket)->socketInstances.size() == 1) {
        for (auto &auth : cmdList) {  // release user auths
            if (auth->getAuthorizedUser() != nullptr) {
                if (auth->getAuthorizedUser() ==
                    UserOperations::getInstance().getUser(clientSocket)->getUserName()) {
                    auth->setAuthorizedUser(nullptr);
                }
            }
        }

        UserOperations::getInstance().removeUser(clientSocket);
        QHostAddress clientAddress(clientSocket->localAddress().toIPv4Address());
        Log()->Info(clientAddress.toString() + " disconnected");
    }
}

void Server::fileTransfer(QTcpSocket *sender, FileTransferCmd *cmd, QByteArray message) {
    fileTransfererSocket = sender;
    isFileTransferInProgress = true;

    message = message.replace('\\', '/');

    int beginningOfFileName = message.lastIndexOf('/');
    transferredFileName = message.mid(beginningOfFileName + 1, message.size());

    transferredFileLocation = cmd->getDestinationDir();
}

bool Server::isAuthorized(QTcpSocket *sender, QString cmdName) {
    if (getCmd(cmdName)->getAuthorizedUser() == nullptr or
        not getCmd(cmdName)->getIsAuthRequired()) {
        return false;
    }

    for (auto &i : UserOperations::getInstance()
                       .getUser(getCmd(cmdName)->getAuthorizedUser())
                       ->socketInstances) {
        if (i == sender) {
            return true;
        }
    }

    return false;
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
    } /*else if (Cmp(message, "getAuth")) {
        authRequest(sender, message);
    } else if (Cmp(message, "clearAuth")) {
        deAuthRequest(sender, message);
    } else if (Cmp(message, "forceAuth")) {
        forceAuth(sender, message);
    } else if (Cmp(message, "createFolder")) {
        createFolder(sender, message);
    }*/
}

void Server::connectProcess(QTcpSocket *sender, QProcess *process) {
    Transmit(sender, "Script execution successful. Forwarding output: ");
    connect(process, &QProcess::readyReadStandardOutput, this,
            [=]() { transmit(sender, process->readLine()); });
}