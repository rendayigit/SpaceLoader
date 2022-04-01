#include "server.h"

int main(int argc, char *argv[]) {
    QCoreApplication a(argc, argv);

    Server::getInstance()->populateCmdLists();

    qint32 serverPort = 1234;
    Server::getInstance()->startServer(serverPort);

    return a.exec();
}