#include "CommonClient.h"

#include <iostream>
#include <vector>

#include "../commonUtil.h"
#include "../constants.h"
#include "../lib/Logger/logger.h"
#include "../path.h"

void CommonClient::onReceived(QByteArray message) {
    qInfo().noquote() << message;

    if (message.contains("Version =")) {
        // float serverVersionNumeric = CommonClientServer::spaceloaderVersion().split(" =
        // ").at(1).toFloat();
        float clientVersionNumeric =
            QString::fromStdString(message.toStdString()).split(" = ").at(1).toFloat();

        // if (serverVersionNumeric < clientVersionNumeric) {
        //     QString message = "WARNING Server version is older than the Client version !!!";
        //     qWarning().noquote() << message;
        //     Log().Warn(message);
        // } else if (serverVersionNumeric > clientVersionNumeric) {
        //     QString message = "WARNING Client version is older than the Server version !!!";
        //     qWarning().noquote() << message;
        //     Log().Warn(message);
        // }
    }
}

void CommonClient::parse(QByteArray message) {
    // if (not parseMessage(nullptr, message)) {
    //     // If the command cannot be found in the client cmds xml then it must be a
    //     // command for the server
    //     transmit(message);
    // }
}

void CommonClient::listenTo(QString msg) {
    int idx1 = msg.indexOf(" ", QString("Listen").size(), Qt::CaseInsensitive) + 1;

    QString ip;
    QString port;

    std::vector<YAML::Node> nodeList = Yaml::getNodeListByKey(
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

    // auto *newclient = new Client();
    // newclient->attemptConnection(ip, port.toInt());

    // listeners.append(newclient);
}

void CommonClient::stopListen(QString ipPort) {
    QString socketAddress;
    QString givenIpPortPair;

    int idx1 = ipPort.indexOf(" ", QString("StopListen").size(), Qt::CaseInsensitive) + 1;

    std::vector<YAML::Node> nodeList =
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

    // for (auto &listener : listeners) {
    //     QHostAddress ip4Address(listener->getSocket()->peerAddress().toIPv4Address());
    //     QString clientAddress = ip4Address.toString();
    //     QString port = QString::number(listener->getSocket()->peerPort());
    //     givenIpPortPair = clientAddress + ":" + port;

    //     if (Cmp(ipPort, givenIpPortPair)) {
    //         listener->disconnect();
    //         delete listener;
    //         listeners.removeOne(listener);
    //     }
    // }
}

void CommonClient::stopAllListeners() {
    // for (auto &listener : listeners) {
    //     listener->getSocket()->abort();
    //     delete listener;
    //     listeners.removeOne(listener);
    // }
}

void CommonClient::fileTransfer(QString localFile, QString serverPath) {
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

void CommonClient::parseInternalCmd([[maybe_unused]] QTcpSocket *sender, QByteArray message) {
    if (Cmp(message, "help")) {
        // std::cout << (" - Client Commands - \n" + CommonClientServer::help()).toStdString();
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
        // string clientVersion = CommonClientServer::spaceloaderVersion().toStdString();
        // cout << "Client " << CommonClientServer::spaceloaderVersion().toStdString() << std::endl;
        std::cout << "Server ";
        transmit("version");
    }
}

void CommonClient::connectProcess(QTcpSocket * /*sender*/, QProcess *process) {
    connect(process, &QProcess::readyReadStandardOutput, this,
            [=]() { std::cout << process->readLine().toStdString() << std::endl; });
}

void CommonClient::onDisconnected() {
    QString message = "Disconnected From Server!";
    qWarning().noquote() << message;
    Log().Warn(message);
}