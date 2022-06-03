#include "backendOperations.h"
#include <QtCore/QFile>
#include <QtCore/QThread>
#include "../lib/TCP/client/tcpClient.h"
#include "../constants.h"
#include "../lib/Logger/logger.h"
#include "iostream"
#include "listener.h"
#include "backend.h"

BackendOperations::BackendOperations(Backend* backend){
    this->backend=backend;
}
void BackendOperations::fileTransfer(QString localFile, QString serverPath){
    // localFile = localFile.replace('"', "");
    // serverPath = serverPath.replace('"', "");

    localFile = localFile.replace("file:///", "");

    qDebug() << "1:" << localFile;
    qDebug() << "2:" << serverPath;

    backend->sendCommand("transmit -s " + localFile.toLocal8Bit() + " -d " + serverPath.toLocal8Bit());

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
            backend->sendCommand(fileData.mid(0, FILETRANSFER_MAX_SINGLE_PACKET_BYTE_SIZE));
            fileData.remove(0, FILETRANSFER_MAX_SINGLE_PACKET_BYTE_SIZE);
        } else {
            backend->sendCommand(fileData.mid(0, fileData.size()));
            fileData.clear();
        }
        iteration++;
        QThread::msleep(30);
        if (iteration % approxfileSize == 0) {
            std::cout << ".";
        }
    }
    backend->sendCommand("#END");

    if (iteration >= 10) {
        std::cout << " Transfer Complete" << std::endl;
    } else {
        std::cout << "An error occured white transferring the file." << std::endl;
        Log().Error("An error occured white transferring file: " + localFile);
    }
}
void BackendOperations::listen(QString ipPort){
    int idx1 = ipPort.indexOf(" ", QString("Listen").size(), Qt::CaseInsensitive) + 1;

    QString ip;
    QString port;

    int idx2 = ipPort.indexOf(":", idx1, Qt::CaseInsensitive);
    int idx3 = ipPort.size();

    ip = ipPort.mid(idx1, idx2 - idx1);
    port = ipPort.mid(idx2 + 1, idx3 - idx2 - 1);

    // listener->attemptConnection(ip, port.toInt());
    // listener->setConnected();
}
void BackendOperations::parse(QString text){
 if (text.contains("Server Logs")) {
        emit backend->clearLogs();
        QStringList logs = text.split("\n");
        logs.removeFirst();
        for (auto& log : logs) {
            emit backend->getLogList(log);
        }
    } else if (text.contains("Reading")) {
        emit backend->getLogText(text);
    } else if (text.contains("User #")) {
        emit backend->getUsers(text);
    }
}
void BackendOperations::start(QString ip){
    backend->attemptConnection(ip, 1234);
    QThread::msleep(100);

    /* Transmit username */
    QByteArray username = getenv("USERNAME");
    backend->sendCommand("addUser " + username);
}