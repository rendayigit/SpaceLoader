#include "ClientConsole.h"

#include <QtConcurrent/QtConcurrent>
#include <QtCore/QCoreApplication>
#include <QtNetwork/QHostAddress>
#include <QtNetwork/QNetworkInterface>
#include <iostream>

#include "../path.h"

ClientConsole::ClientConsole()
    : CommonClient(), CommonClientServer(Paths().getClientCmdsYaml()), noloop(false) {}

void ClientConsole::loopForCommands() {
    while (true) {
        QTextStream s(stdin);
        QByteArray consoleInput = s.readLine().toLocal8Bit();

        emit consoleInputReceived(consoleInput);
    }
}

void ClientConsole::start(QList<QString> commandArguments) {
    connect(this, &ClientConsole::consoleInputReceived, this, &CommonClient::parse);

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
                cout << "Waiting for " << timeout.toStdString() << " seconds before exiting..."
                     << endl;
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
    QThread::msleep(300);
    transmit("version");

    QtConcurrent::run(this, &ClientConsole::loopForCommands);
}
