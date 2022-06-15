#include "client.h"

int main(int argc, char *argv[]) {
    QCoreApplication a(argc, argv);

    /* Parse arguments */
    QList<QString> commandArguments;
    if (argc > 1) {                       // if there is an argument passed
        for (int i = 1; i < argc; i++) {  // first argument is always the executable name
            commandArguments << argv[i];
        }
    }

    /* If the dev is debugging the app and forgot
     * to pass the IP as an argument this passes
     * localhost as the IP
     */
#ifdef QT_DEBUG
    if (commandArguments.isEmpty()) {
        commandArguments.append("127.0.0.1");
    }
#endif

    if (commandArguments.isEmpty()) {
        std::cout << "Enter Server IP: ";
        std::string ip;
        std::cin >> ip;
        commandArguments.append(QString::fromStdString(ip));
    }

    /* Connect to server */
    Client::getInstance().start(commandArguments);

    Client::getInstance().stopAllListeners();

    return a.exec();
}
