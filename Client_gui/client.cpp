#include "client.h"

#include "../common.h"
#include "../path.h"
#include "backend.h"

using namespace std;

Client::Client() : Operations(Paths().getClientCmdsYaml()) {}

void Client::onReceived(QByteArray message) {
    std::cout << message.toStdString() << std::endl;

    emit Backend::getInstance().getReceivedText(message);
}

void Client::onDisconnected() { std::cout << "Disconnected From Server!"; }

void Client::parse(QByteArray message) {
    if (not parseMessage(nullptr, message)) {
        // If the command cannot be found in the client cmds xml then it must be a
        // command for the server
        sendCommand(message);
    }
}

void Client::start(QString ip) {
    connect(this, &Client::consoleInputReceived, this, &Client::parse);

    populateCmdLists();

    attemptConnection(ip, 1234);
    
    /* Transmit username */
    QByteArray username = getenv("USERNAME");
    sendCommand("addUser " + username);
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
        listener->disconnect();
        delete listener;
        listeners.removeOne(listener);
    }
}

void Client::fileTransfer(QTcpSocket * /*sender*/, FileTransferCmd * /*cmd*/, QByteArray message) {
    sendCommand(message);
    QThread::msleep(10);

    int endOfCmdName = message.indexOf(" ");
    QString fileNamePath = message.mid(endOfCmdName + 1, message.size());

    QFile file(fileNamePath);
    file.open(QIODevice::ReadOnly);
    fileNamePath.remove(fileNamePath.size() - 1);
    QByteArray fileData = file.readAll();
    if (fileData.isNull()) {
        std::cout << "An error occured: Cannot open the provided file" << std::endl;
        Log()->Error("Error opening" + fileNamePath);
        return;
    }
    std::cout << "File size: " << fileData.size() / BYTE_TO_KILOBYTE << "KiloByte" << std::endl;
    int approxfileSize = fileData.size() / (FILETRANSFER_MAX_SINGLE_PACKET_BYTE_SIZE * 10) + 1;
    std::cout << "Progress: ";
    int iteration = 0;
    while (!fileData.isNull()) {
        if (fileData.size() >= FILETRANSFER_MAX_SINGLE_PACKET_BYTE_SIZE) {
            sendCommand(fileData.mid(0, FILETRANSFER_MAX_SINGLE_PACKET_BYTE_SIZE));
            fileData.remove(0, FILETRANSFER_MAX_SINGLE_PACKET_BYTE_SIZE);
        } else {
            sendCommand(fileData.mid(0, fileData.size()));
            fileData.clear();
        }
        iteration++;
        QThread::msleep(30);
        if (iteration % approxfileSize == 0) {
            std::cout << ".";
        }
    }
    sendCommand("#END");

    if (iteration >= 10) {
        std::cout << " Transfer Complete" << std::endl;
    } else {
        std::cout << "An error occured white transferring the file." << std::endl;
        Log()->Error("An error occured white transferring file: " + fileNamePath);
    }
}

void Client::parseInternalCmd([[maybe_unused]] QTcpSocket *sender, QByteArray message) {
    if (Cmp(message, "help")) {
        std::cout << (" - Client Commands - \n" + Operations::help()).toStdString();
        sendCommand("help");
    } else if (Cmp(message, "ListenTo")) {
        listenTo(message);
    } else if (Cmp(message, "StopListeningTo")) {
        stopListen(message);
    } else if (Cmp(message, "getAuth")) {
        sendCommand(message);
    }
}

void Client::connectProcess(QTcpSocket * /*sender*/, QProcess *process) {
    connect(process, &QProcess::readyReadStandardOutput, this,
            [=]() { std::cout << process->readLine().toStdString() << std::endl; });
}
