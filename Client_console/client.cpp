#include "client.h"
#include "../common.h"
#include "../path.h"

using namespace std;

Client *Client::m_instance = Client::getInstance();

Client::Client() : Operations(Paths().getClientCmdsYaml()) {}

Client *Client::getInstance() {
    if (Client::m_instance == nullptr) {
        Client::m_instance = new Client;
    }
    return Client::m_instance;
}

void Client::onReceived(QByteArray message) {
    QString messageToPrint = message;
    if (message.contains("help")) {
        messageToPrint = helpFormatter(messageToPrint);
        std::cout << messageToPrint.toStdString() << std::endl;
    } else {
        std::cout << message.toStdString() << std::endl;
    }
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
        m_instance->sendCommand(message);
    }
}

void Client::start(QList<QString> commandArguments) {
    connect(m_instance, &Client::consoleInputReceived, m_instance, &Client::parse);
	
    populateCmdLists();

    while (!m_instance->attemptConnection(commandArguments.first(), 1234)) {
        commandArguments.removeFirst();
        std::cout << "Connection error please re-enter IP: ";
        std::string ip;
        std::cin >> ip;
        commandArguments.insert(0, QString::fromStdString(ip));
    }
    commandArguments.removeFirst();

    /* Transmit username */
    QByteArray username = getenv("USERNAME");
    QHostAddress ip(getSocket()->localAddress().toIPv4Address());
    m_instance->sendCommand("addUser " + username);

    while (!commandArguments.isEmpty()) {
        QThread::usleep(100);
        QByteArray value = commandArguments.first().toLocal8Bit();
        commandArguments.removeFirst();

        emit consoleInputReceived(value);
    }

    QtConcurrent::run(m_instance, &Client::loopForCommands);
}

void Client::listenTo(QString msg) {
    int idx1 = msg.indexOf(" ", QString("ListenTo").size(), Qt::CaseInsensitive) + 1;

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

    int idx1 = ipPort.indexOf(" ", QString("StopListeningTo").size(), Qt::CaseInsensitive) + 1;

    vector<YAML::Node> nodeList = Yaml::getNodeListByKey(
        Paths().getConfigYaml().toStdString(), "name", ipPort.mid(idx1, ipPort.size()).toStdString());
    if (nodeList.size() < 1) {
        int idx2 = ipPort.indexOf(":", idx1, Qt::CaseInsensitive);
        int idx3 = ipPort.size();

        ipPort = ipPort.mid(idx1, idx2 - idx1) + ":" + ipPort.mid(idx2 + 1, idx3 - idx2 - 1);
    } else {
        ipPort = QString::fromStdString(Yaml::getValue(nodeList.at(0), "ip")) +
                 ":" +
                 QString::fromStdString(Yaml::getValue(nodeList.at(0), "port"));
    }

    for (auto &listener : listeners) {
        QHostAddress ip4Address(listener->getSocket()->peerAddress().toIPv4Address());
        QString clientAddress = ip4Address.toString();
        QString port = QString::number(listener->getSocket()->peerPort());
        givenIpPortPair = clientAddress + ":" + port;

        if (cmp(ipPort, givenIpPortPair)) {
            listener->disconnect();
            delete listener;
            listeners.removeOne(listener);
        }
    }
}

void Client::help(QByteArray message) {
    if (cmp("help", message)) {
        std::cout << "\nClient command list:\n" << std::endl;
        for (auto &cmd : cmdList) {
            QString helpString = "\t" + cmd->getCmdCallString();
            helpString.insert(25, "- " + cmd->getCmdDescription());
            helpString = helpFormatter(helpString);
            std::cout << helpString.toStdString() << std::endl;
        }
        m_instance->sendCommand(message);
    } else {
        QString requestedCmd = message.mid(message.indexOf("help ") + 5, message.size());
        requestedCmd = requestedCmd.toLower();
        vector<YAML::Node> nodeList = Yaml::getNodeListByKey(
            Paths().getClientCmdsYaml().toStdString(), "id", requestedCmd.toStdString());
        if (nodeList.size() < 1) {
            m_instance->sendCommand(message);
            return;
        }

        QString helpString = "\t" + QString::fromStdString(Yaml::getValue(
                                        nodeList.at(0), "Description"));
        helpString.insert(25, "- " + QString::fromStdString(
                                         Yaml::getValue(nodeList.at(0), "type")));
        helpString = helpFormatter(helpString);
        std::cout << helpString.toStdString() << std::endl;
    }
}

void Client::stopAllListeners() {
    for (auto &listener : listeners) {
        listener->disconnect();
        delete listener;
        listeners.removeOne(listener);
    }
}

void Client::fileTransfer(QTcpSocket * /*sender*/, FileTransferCmd * /*cmd*/, QByteArray message) {
    m_instance->sendCommand(message);
    QThread::msleep(10);

    int endOfCmdName = message.indexOf(" ");
    QString fileNamePath = message.mid(endOfCmdName + 1, message.size());

    QFile file(fileNamePath);
    file.open(QIODevice::ReadOnly);
    fileNamePath.remove(fileNamePath.size() - 1);
    QByteArray fileData = file.readAll();
    if (fileData.isNull()) {
        std::cout << "An error occured: Cannot open the provided file" << std::endl;
        Log().Error("Error opening" + fileNamePath);
        return;
    }
    std::cout << "File size: " << fileData.size() / BYTE_TO_KILOBYTE << "KiloByte" << std::endl;
    int approxfileSize = fileData.size() / (FILETRANSFER_MAX_SINGLE_PACKET_BYTE_SIZE * 10) + 1;
    std::cout << "Progress: ";
    int iteration = 0;
    while (!fileData.isNull()) {
        if (fileData.size() >= FILETRANSFER_MAX_SINGLE_PACKET_BYTE_SIZE) {
            m_instance->sendCommand(fileData.mid(0, FILETRANSFER_MAX_SINGLE_PACKET_BYTE_SIZE));
            fileData.remove(0, FILETRANSFER_MAX_SINGLE_PACKET_BYTE_SIZE);
        } else {
            m_instance->sendCommand(fileData.mid(0, fileData.size()));
            fileData.clear();
        }
        iteration++;
        QThread::msleep(30);
        if (iteration % approxfileSize == 0) {
            std::cout << ".";
        }
    }
    m_instance->sendCommand("#END");

    if (iteration >= 10) {
        std::cout << " Transfer Complete" << std::endl;
    } else {
        std::cout << "An error occured white transferring the file." << std::endl;
        Log().Error("An error occured white transferring file: " + fileNamePath);
    }
}

void Client::parseInternalCmd([[maybe_unused]] QTcpSocket *sender, QByteArray message) {
    if (cmp(message, "help")) {
        help(message);
    } else if (cmp(message, "ListenTo")) {
        listenTo(message);
    } else if (cmp(message, "StopListeningTo")) {
        stopListen(message);
    } else if (cmp(message, "getAuth")) {
        m_instance->sendCommand(message);
    }
}

void Client::connectProcess(QTcpSocket * /*sender*/, QProcess *process) {
    connect(process, &QProcess::readyReadStandardOutput, this,
            [=]() { std::cout << process->readLine().toStdString() << std::endl; });
}
