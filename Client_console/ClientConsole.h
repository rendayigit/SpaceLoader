#ifndef CLIENTCONSOLE_H
#define CLIENTCONSOLE_H

#include <QtCore/qobjectdefs.h>
#include "../Common/CommonClient.h"
#include "../Common/CommonClientServer.h"

class ClientConsole : public CommonClient, public CommonClientServer {
    Q_OBJECT

   public:
    static auto &getInstance() {
        static ClientConsole instance;
        return instance;
    }

    void start(QList<QString> commandArguments);

    /**
     * @brief Continuously read commands from the console
     *
     */
    void loopForCommands();

    void parseInternalCmd([[maybe_unused]] QTcpSocket *sender, QByteArray message) override {}
    void connectProcess(QTcpSocket * /*sender*/, QProcess *process) override {}

   signals:
    void consoleInputReceived(QByteArray input);

   private:
    ClientConsole();
    bool noloop;
};

#endif  // CLIENTCONSOLE_H