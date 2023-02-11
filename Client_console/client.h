#ifndef CLIENT_H
#define CLIENT_H

#include "../Operations/Operations.h"
#include "../lib/TCP/client/tcpClient.h"

class Client : public TCPClient, public Operations {
    Q_OBJECT
   public:
    static auto &getInstance() {
        static Client instance;
        return instance;
    }

    void onReceived(QByteArray message) override;
    void onDisconnected() override;
    void start(QList<QString> commandArguments);
    void stopAllListeners();

    void fileTransfer(QString localFile, QString serverPath);
    void parseInternalCmd(QTcpSocket *sender, QByteArray message);
    void connectProcess(QTcpSocket *sender, QProcess *process);

    void handleNullCmd(QByteArray message);
    void help(QByteArray message);  // TODO- was private before

   private:
    void listenTo(QString msg);
    void stopListen(QString ipPort);

    /**
     * @brief Continuously read commands from the console
     *
     */
    void loopForCommands();

   public slots:
    void parse(QByteArray message);

   signals:
    void consoleInputReceived(QByteArray input);

   private:
    Client();
    QList<Client *> listeners;
    bool noloop = false;
};

#endif  // CLIENT_H
