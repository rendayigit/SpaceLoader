#ifndef BACKEND_H
#define BACKEND_H

#include "../lib/TCP/client/tcpClient.h"

class Backend : public TCPClient {
    Q_OBJECT

   public:
    Backend();
    void onReceived(QByteArray message);
    void onDisconnected();

   public slots:
    Q_INVOKABLE void getTerminalData(QString text);
    Q_INVOKABLE void start();
    Q_INVOKABLE void selectLogFile(QString fileName);
    Q_INVOKABLE void listLogs();
    Q_INVOKABLE void getUserList();
    Q_INVOKABLE void fileTransfer(QString localFile, QString serverPath);
    Q_INVOKABLE void listen(QString ipPort);
    Q_INVOKABLE void stopListen();
    Q_INVOKABLE void setServerIp(QString ip);
    Q_INVOKABLE QString getLocalIp();

   signals:
    void getReceivedText(QString text);
    void getLogList(QString text);
    void getLogText(QString text);
    void clearLogs();
    void getUsers(QString text);
    void getListenerText(QString text);

   private:
    void parse(QString text);
    QString serverIp = "";
    QString localIp = "";
};

#endif  // BACKEND_H