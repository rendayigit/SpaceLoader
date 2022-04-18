#include "server.h"

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

void Server::addUser(QTcpSocket *sender, QByteArray message) {
    message.replace("username ", "");
    QString username = message.mid(0, message.indexOf(" "));
    QHostAddress ip(sender->localAddress().toIPv4Address());

    if (getUser(username) == nullptr)
        userList.append(new User(username, sender));
    else
        getUser(username)->socketInstances.append(sender);

    Log()->Event(username + " (" + ip.toString() + ") connected.");
}

void Server::getUserList(QTcpSocket *sender) {
    QString users = "";

    for (int i = 0; i < userList.size(); i++) {
        QHostAddress ip(userList.at(i)->socketInstances.at(0)->localAddress().toIPv4Address());
        users += "User #" + QString::number(i);
        users += " username: " + userList.at(i)->getUserName() + '\n';
        users += "User #" + QString::number(i);
        users += " ip: " + ip.toString() + '\n';
    }

    Transmit(sender, users.toLocal8Bit());
}

void Server::clientDisconnected(QTcpSocket *clientSocket) {
    if (getUser(clientSocket)->socketInstances.size() == 1) {
        for (auto &auth : cmdList) {  // release user auths
            if (auth->getAuthorizedUser() != nullptr) {
                if (auth->getAuthorizedUser() == getUser(clientSocket)->getUserName()) {
                    auth->setAuthorizedUser(nullptr);
                }
            }
        }

        userList.removeOne(getUser(clientSocket));
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

// TODO move to /Commands
bool Server::isAuthorized(QTcpSocket *sender, QString cmdName) {
    if (getCmd(cmdName)->getAuthorizedUser() == nullptr ||
        getCmd(cmdName)->getIsAuthRequired() == false)
        return false;
    else
        for (auto &i : getUser(getCmd(cmdName)->getAuthorizedUser())->socketInstances) {
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

User *Server::getUser(QString userName) {
    for (auto &i : userList) {
        if (i->getUserName() == userName) return i;
    }

    return nullptr;
}

User *Server::getUser(QTcpSocket *socket) {
    for (auto &i : userList) {
        for (auto &j : i->socketInstances) {
            if (j == socket) return i;
        }
    }

    return nullptr;
}

QList<QString> Server::getDlibs(QString path) {
    QList<QString> libList;

    QDirIterator iterator(path, QDirIterator::Subdirectories);

    while (iterator.hasNext()) {
        QFile file(iterator.next());
        if (file.open(QIODevice::ReadOnly) and QLibrary::isLibrary(file.fileName())) {
            libList.append(file.fileName());
        }
    }

    return libList;
}

void Server::runDynamicCmd(QTcpSocket *sender, QByteArray message) {
    QList<QString> commandLibs = getDlibs(Paths().getCmdsDir());
    if (commandLibs.isEmpty()) {
        Log()->Error("no libs found at " + Paths().getCmdsDir());
    } else {
        for (auto &lib : commandLibs) {
            if (lib.contains(GetCmd(message), Qt::CaseInsensitive)) {
                QPluginLoader loader(lib);
                if (auto *instance = loader.instance()) {
                    if (auto *plugin = qobject_cast<CmdPluginInterface *>(instance)) {
                        plugin->run(sender, message);
                    } else {
                        Log()->Error("qobject_cast<> returned nullptr");
                    }
                } else {
                    Log()->Error(loader.errorString());
                }
            }
        }
    }
}

void Server::parseInternalCmd(QTcpSocket *sender, QByteArray message) {
    if(Cmp(message, "addUser")) {
        addUser(sender, message);
    } else if(Cmp(message, "getUserList")) {
        getUserList(sender);
    }
}

void Server::connectProcess(QTcpSocket *sender, QProcess *process) {
    transmit(sender, "Script execution successful. Forwarding output: ");
    connect(process, &QProcess::readyReadStandardOutput, this,
            [=]() { transmit(sender, process->readLine()); });
}