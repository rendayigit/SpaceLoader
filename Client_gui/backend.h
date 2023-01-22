#ifndef BACKEND_H
#define BACKEND_H

#include "../lib/TCP/client/tcpClient.h"  // TODO - dont use relative include paths

class Backend : public TCPClient {
    Q_OBJECT

   public:
    Backend();
    void onReceived(QByteArray message) override;
    void onDisconnected() override;
    void egseReplier(QString message);
    void egseDisconnectedError();

   public slots:
    Q_INVOKABLE void getTerminalData(QString text);
    Q_INVOKABLE void start();
    Q_INVOKABLE void selectLogFile(QString fileName);
    Q_INVOKABLE void listLogs();
    Q_INVOKABLE void getUserList();
    Q_INVOKABLE static void listen(QString ipPort);
    Q_INVOKABLE static void stopListen();
    Q_INVOKABLE void setServerIp(QString ip);
    Q_INVOKABLE QString getLocalIp();
    Q_INVOKABLE void updateYamlFile(QString path, QString value);
    Q_INVOKABLE QString getConfigValue(QString path, QString key);
    Q_INVOKABLE void transmitEgseTc(QString tc);
    Q_INVOKABLE void egseConnect(QString deviceIp, QString devicePort);
    Q_INVOKABLE int fileTransfer(QString localFile, QString serverPath);

   signals:
    void getReceivedText(QString text);
    void getLogList(QString text);
    void getLogText(QString text);
    void clearLogs();
    void getUsers(QString text);
    void getListenerText(QString text);
    void egseError(bool isError, QString text);
    void egseReply(QString message);
    void setTransferProgress(bool isProgressing);
    void setTransferError(bool isError, QString errorMessage);

   private:
    void parse(QString text);

    QString serverIp = "";
    QString localIp = "";
};

#endif  // BACKEND_H
