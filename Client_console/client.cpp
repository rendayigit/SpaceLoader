#include "client.h"

#include "../common.h"
#include "../path.h"

using namespace std;

Client::Client() : Operations(Paths().getClientCmdsYaml()) {}
std::string serverVersion;

void Client::onReceived(QByteArray message) {
    if (message.toStdString().find("Version =") != std::string::npos) {
        serverVersion = message.toStdString();
    }
    std::cout << message.toStdString() << std::endl;
}

void Client::onDisconnected() { std::cout << "Disconnected From Server!"; }

void Client::loopForCommands() {
    while (true) {
        QTextStream s(stdin);
        QByteArray consoleInput = s.readLine().toLocal8Bit();

        emit consoleInputReceived(consoleInput);
    }
}

void Client::parse(QByteArray message) {
    if (not parseMessage(nullptr, message)) {
        // If the command cannot be found in the client cmds xml then it must be a
        // command for the server
        transmit(message);
    }
}

void Client::start(QList<QString> commandArguments) {
    connect(this, &Client::consoleInputReceived, this, &Client::parse);

    populateCmdLists();

    while (not attemptConnection(commandArguments.first(), 1234)) {
        commandArguments.removeFirst();

        for (auto c : commandArguments) {
            if (c.contains("noloop")) {
                exit(0);
            }
        }

        std::cout << "Connection error please re-enter IP: ";
        std::string ip;
        std::cin >> ip;
        commandArguments.insert(0, QString::fromStdString(ip));
    }
    commandArguments.removeFirst();

    /* Transmit username */
    QByteArray username = getenv("USERNAME");
    QHostAddress ip(getSocket()->localAddress().toIPv4Address());
    transmit("addUser " + username);
    QThread::usleep(250);
    getSocket()->flush();
    QThread::usleep(250);

    while (not commandArguments.isEmpty()) {
        QString command = commandArguments.first();
        commandArguments.removeFirst();
        QThread::usleep(100);
        if (command.contains("noloop", Qt::CaseInsensitive)) {
            noloop = true;
            int idx = command.indexOf("noloop", 0, Qt::CaseInsensitive) + 7;
            QString timeout = command.mid(idx, command.length() - idx);
            if (timeout.size() > 0) {
                std::cout << "Waiting for " << timeout.toStdString() << " seconds before exiting..."
                          << std::endl;
                QThread::sleep(timeout.toInt());
            }
        } else {
            emit consoleInputReceived(command.toLocal8Bit());
        }
    }

    if (noloop) {
        exit(0);
    }

    transmit("getuserlist");
    transmit("version");

    QtConcurrent::run(this, &Client::loopForCommands);
}

void Client::listenTo(QString msg) {
    int idx1 = msg.indexOf(" ", QString("Listen").size(), Qt::CaseInsensitive) + 1;

    QString ip;
    QString port;

    vector<YAML::Node> nodeList = Yaml::getNodeListByKey(
        Paths().getConfigYaml().toStdString(), "name", msg.mid(idx1, msg.size()).toStdString());
    if (nodeList.size() < 1) {
        int idx2 = msg.indexOf(":", idx1, Qt::CaseInsensitive);
        int idx3 = msg.size();

        ip = msg.mid(idx1, idx2 - idx1);
        port = msg.mid(idx2 + 1, idx3 - idx2 - 1);
    } else {
        ip = QString::fromStdString(Yaml::getValue(nodeList.at(0), "ip"));
        port = QString::fromStdString(Yaml::getValue(nodeList.at(0), "port"));
    }

    auto *newclient = new Client();
    newclient->attemptConnection(ip, port.toInt());

    listeners.append(newclient);
}

void Client::stopListen(QString ipPort) {
    QString socketAddress;
    QString givenIpPortPair;

    int idx1 = ipPort.indexOf(" ", QString("StopListen").size(), Qt::CaseInsensitive) + 1;

    vector<YAML::Node> nodeList =
        Yaml::getNodeListByKey(Paths().getConfigYaml().toStdString(), "name",
                               ipPort.mid(idx1, ipPort.size()).toStdString());
    if (nodeList.size() < 1) {
        int idx2 = ipPort.indexOf(":", idx1, Qt::CaseInsensitive);
        int idx3 = ipPort.size();

        ipPort = ipPort.mid(idx1, idx2 - idx1) + ":" + ipPort.mid(idx2 + 1, idx3 - idx2 - 1);
    } else {
        ipPort = QString::fromStdString(Yaml::getValue(nodeList.at(0), "ip")) + ":" +
                 QString::fromStdString(Yaml::getValue(nodeList.at(0), "port"));
    }

    for (auto &listener : listeners) {
        QHostAddress ip4Address(listener->getSocket()->peerAddress().toIPv4Address());
        QString clientAddress = ip4Address.toString();
        QString port = QString::number(listener->getSocket()->peerPort());
        givenIpPortPair = clientAddress + ":" + port;

        if (Cmp(ipPort, givenIpPortPair)) {
            listener->disconnect();
            delete listener;
            listeners.removeOne(listener);
        }
    }
}

void Client::stopAllListeners() {
    for (auto &listener : listeners) {
        listener->getSocket()->abort();
        delete listener;
        listeners.removeOne(listener);
    }
}

void Client::fileTransfer(QString localFile, QString serverPath) {
    transmit("transmit -s " + localFile.toLocal8Bit() + " -d " + serverPath.toLocal8Bit());
    QThread::msleep(10);

    QFile file(localFile);
    file.open(QIODevice::ReadOnly);
    QByteArray fileData = file.readAll();
    if (fileData.isNull()) {
        std::cout << "An error occured: Cannot open the provided file" << std::endl;
        Log().Error("Error opening" + localFile);
        return;
    }
    std::cout << "File size: " << fileData.size() / BYTE_TO_KILOBYTE << "KiloByte" << std::endl;
    int approxfileSize = fileData.size() / (FILETRANSFER_MAX_SINGLE_PACKET_BYTE_SIZE * 10) + 1;
    std::cout << "Progress: ";
    int iteration = 0;
    while (!fileData.isNull()) {
        if (fileData.size() >= FILETRANSFER_MAX_SINGLE_PACKET_BYTE_SIZE) {
            transmit(fileData.mid(0, FILETRANSFER_MAX_SINGLE_PACKET_BYTE_SIZE));
            fileData.remove(0, FILETRANSFER_MAX_SINGLE_PACKET_BYTE_SIZE);
        } else {
            transmit(fileData.mid(0, fileData.size()));
            fileData.clear();
        }
        iteration++;
        QThread::msleep(30);
        if (iteration % approxfileSize == 0) {
            std::cout << ".";
        }
    }
    transmit("#END");

    if (iteration >= 10) {
        std::cout << " Transfer Complete" << std::endl;
    } else {
        std::cout << "An error occured white transferring the file." << std::endl;
        Log().Error("An error occured white transferring file: " + localFile);
    }
}

void Client::parseInternalCmd([[maybe_unused]] QTcpSocket *sender, QByteArray message) {
    if (Cmp(message, "help")) {
        std::cout << (" - Client Commands - \n" + Operations::help()).toStdString();
        transmit("help");
    } else if (Cmp(message, "exit")) {
        exit(0);
    } else if (Cmp(message, "Listen")) {
        listenTo(message);
    } else if (Cmp(message, "StopListen")) {
        stopListen(message);
    } else if (Cmp(message, "transmit")) {
        message = message.simplified();

        int sIndex = message.indexOf("-s", 0) + 3;
        int dIndex = message.indexOf("-d") - 1;

        QString localFileAndPath = message.mid(sIndex, dIndex - sIndex).simplified();
        QString serverPath = message.mid(dIndex + 4, message.length()).simplified();

        std::cout << "localFileAndPath: " << localFileAndPath.toStdString() << std::endl;
        std::cout << "serverPath: " << serverPath.toStdString() << std::endl;

        fileTransfer(localFileAndPath, serverPath);
    } else if (Cmp(message, "version")) {
        string clientVersion = Operations::spaceloaderVersion().toStdString();

        std::cout << "Client " << clientVersion << std::endl;
        std::cout << "Server " << serverVersion << std::endl;
        // transmit("version");

        // vector<string> temp;
        // stringstream ss(clientVersion);
        // string item; // item is a string which contains a part of the string

        // while (std::getline(ss, item, '=')) {
        //     temp.push_back(item);
        // }

        // float serverVersionFloat = std::stof(temp.back().erase(0.1));
        // std::cout << temp.back().erase(0.1);
        // float clientVersionFloat = std::stof(clientVersion);

        // if (serverVersionFloat < clientVersionFloat) {
        //     std::cout << "WARNING Client Version is older than the Server Version !!!" << std::endl;
        // } else {
        //     std::cout << "WARNING Server Version is older than the Client Version !!!" << std::endl;
        // }
    }
}

void Client::connectProcess(QTcpSocket * /*sender*/, QProcess *process) {
    connect(process, &QProcess::readyReadStandardOutput, this,
            [=]() { std::cout << process->readLine().toStdString() << std::endl; });
}
