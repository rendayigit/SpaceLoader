#ifndef BACKEND_H
#define BACKEND_H

#include "../lib/TCP/client/tcpClient.h"  // TODO - dont use relative include paths
#include "Scoc3/treeNode.h"

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
    Q_INVOKABLE void transmitEgseTc(QString tc);
    Q_INVOKABLE void egseConnect(QString deviceIp, QString devicePort);
    Q_INVOKABLE int fileTransfer(QString localFile, QString serverPath);
    /**
     * @brief Get the list of files in "./src/reg".
     * @return QList<QString>
     */
    Q_INVOKABLE QList<QString> getFileList();
    Q_INVOKABLE QList<QString> getConfFileList();

    /**
     * @brief Set the file path to the certain .yaml file in "./src/reg" according to the moduleId
     * given.
     * @param moduleId
     */
    Q_INVOKABLE void setFilePath(int moduleId);
    Q_INVOKABLE void setConfFilePath(int configId);
    Q_INVOKABLE int returnSelectedConfigId();
    Q_INVOKABLE void resetConfigId();

    /**
     * @brief Get the list of registers from the chosen .yaml file.
     * @return QList<QString>
     */
    Q_INVOKABLE QList<QString> getRegisterList();

    /**
     * @brief Get the list of the field from the given register ID.
     * @param regId
     * @return QList<QString>
     */
    Q_INVOKABLE QList<QString> getFieldList(QString regId);

    /**
     * @brief Get the configuration type from the given fieldId. ( ConfType: [-1]->Not applicable,
     * read-only  [0]->Combo Box  [1]->Text Box )
     * @param fieldId
     * @return int
     */
    Q_INVOKABLE int getConfType(QString fieldId);
    Q_INVOKABLE int getReadable(QString fieldId);
    Q_INVOKABLE int getWriteable(QString fieldId);
    Q_INVOKABLE QString getResetValue(QString fieldId);
    Q_INVOKABLE QList<QString> getValueDescriptions(QString fieldId);
    Q_INVOKABLE QString getFieldAddr();
    Q_INVOKABLE void saveConfig(QString writeValue, int base);
    Q_INVOKABLE QString getValueFromConfigFile();
    Q_INVOKABLE QString returnHex(QString num);
    Q_INVOKABLE void sshSet(QString address, QString value);
    Q_INVOKABLE QString sshGet(QString address);
    Q_INVOKABLE int returnGlobalModuleId();
    Q_INVOKABLE QString returnGlobalRegId();
    Q_INVOKABLE QString returnGlobalFieldId();
    Q_INVOKABLE QString returnGlobalConfigId();
    Q_INVOKABLE void setDefaultConfigId(QString configName);
    Q_INVOKABLE void setGlobalModuleId(int moduleId);
    Q_INVOKABLE void setGlobalRegId(int regId);
    Q_INVOKABLE void setGlobalFieldId(int fieldId);
    Q_INVOKABLE int checkAllConfigValues(int mode, QString check = "");
    Q_INVOKABLE void checkAndSaveAll(QString newFileName);

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
    void print(QString text);
    void log(QString log);

   private:
    void parse(QString text);
    QList<QString> vectorToQList(std::vector<std::string> vector);
    int getRangeStart(std::string str);
    int countSpaces(std::string data);
    std::string deleteNonAlphaNumerical(std::string data);
    std::vector<std::string> deleteNonAlphaNumerical_Field(std::string data);
    int getIdByName(std::string component, std::string name);
    std::string getFieldAddrByPath(std::string path);
    bool getIsFieldWriteOnlyByPath(std::string path);
    int searchNodeVector(std::vector<TreeNode> container, std::string key);
    TreeNode parseConfig(std::string configFilePath);

    QString serverIp = "";
    QString localIp = "";
    int globalModuleId = -1;
    QString globalRegId = "-1";
    QString globalFieldId = "-1";
    int globalConfigId = 0;
    std::string filePath;
    std::string configFilePath = "../src/conf/default.yaml";
};

#endif  // BACKEND_H
