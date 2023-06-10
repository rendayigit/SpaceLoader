#include "backend.h"

#include <fcntl.h>
#include <libssh/libssh.h>
#include <libssh/sftp.h>
#include <sys/stat.h>

#include <QtConcurrent/QtConcurrent>
#include <QtCore/QDir>
#include <QtCore/QThread>
#include <filesystem>
#include <fstream>

#include "../common.h"
#include "../lib/Logger/logger.h"
#include "../lib/YAML/yaml.h"
#include "egse.h"
#include "listener.h"


Listener *listener;
Egse *egse;

using namespace std;
namespace fs = std::filesystem;

Backend::Backend() : localIp(GetLocalIp().last()) {
    listener = new Listener(this);
    egse = new Egse(this);
}

void Backend::onReceived(QByteArray message) {
    parse(message);
    emit getReceivedText(message);
}

void Backend::onDisconnected() { Log().Error("Disconnected From Server!"); }

void Backend::egseReplier(QString message) { emit egseReply(message); }

void Backend::getTerminalData(QString text) {
    transmit(text.mid(text.lastIndexOf("\n> ") + 3, text.size()).toLocal8Bit());
}

void Backend::start() {
    attemptConnection(serverIp, 1234);
    QThread::msleep(100);
    // TODO - This delay is important. Consider adding same delay for console client

    /* Transmit username */
    QByteArray username = getenv("USERNAME");
    transmit("addUser " + username);
}

void Backend::selectLogFile(QString fileName) { transmit("readLog " + fileName.toLocal8Bit()); }

void Backend::listLogs() { transmit("listLogs"); }

void Backend::getUserList() { transmit("getUserList"); }

void Backend::listen(QString ipPort) {
    int idx1 = ipPort.indexOf(" ", QString("Listen").size(), Qt::CaseInsensitive) + 1;

    QString ip;
    QString port;

    int idx2 = ipPort.indexOf(":", idx1, Qt::CaseInsensitive);
    int idx3 = ipPort.size();

    ip = ipPort.mid(idx1, idx2 - idx1);
    port = ipPort.mid(idx2 + 1, idx3 - idx2 - 1);

    listener->attemptConnection(ip, port.toInt());
    listener->setConnected();
}

void Backend::stopListen() { listener->disconnect(); }

void Backend::setServerIp(QString ip) { serverIp = ip; }

QString Backend::getLocalIp() { return localIp; }

void Backend::transmitEgseTc(QString tc) {
    if (egse->isConnected()) {
        egse->buttonCallback(tc);
    } else {
        egseDisconnectedError();
    }
}

void Backend::egseConnect(QString deviceIp, QString devicePort) {
    if (egse->attemptConnection(deviceIp, devicePort.toInt())) {
        emit egseError(false, "Online, Connected");
        egse->setConnected(true);
    } else {
        egseDisconnectedError();
    }
}

void Backend::egseDisconnectedError() {
    QString errorMessage = "ERROR! Not Connected";
    egse->setConnected(false);
    emit egseError(true, errorMessage);
    Log().Error(errorMessage);
}

void Backend::parse(QString text) {
    if (text.contains("Server Logs")) {
        emit clearLogs();
        QStringList logs = text.split("\n");
        logs.removeFirst();
        for (auto &log : logs) {
            emit getLogList(log);
        }
    } else if (text.contains("Reading")) {
        emit getLogText(text);
    } else if (text.contains("User #")) {
        emit getUsers(text);
    }
}

// TODO - implement to console client as well.
// TODO - Log ssh operations on the server side as well and not just the client side.
int Backend::fileTransfer(QString localFile, QString serverPath) {
    QtConcurrent::run([=]() {
        emit setTransferProgress(true);

        ssh_session session = ssh_new();

        ssh_options_set(session, SSH_OPTIONS_HOST, "192.168.1.2");
        ssh_options_set(session, SSH_OPTIONS_USER, "Administrator");

        int rc = ssh_connect(session);

        if (rc != SSH_OK) {
            QString errorMessage =
                "Error connecting to host: " + QString::fromStdString(ssh_get_error(session));
            Log().Error(errorMessage);
            emit setTransferError(true, errorMessage);
            return -1;
        }

        rc = ssh_userauth_password(session, "Administrator", "uyssw");
        if (rc != SSH_OK) {
            QString errorMessage = "Error authenticating with password: " +
                                   QString::fromStdString(ssh_get_error(session));
            Log().Error(errorMessage);
            emit setTransferError(true, errorMessage);
            return -1;
        }

        std::ifstream file(localFile.toStdString(), std::ios::binary);
        if (!file.is_open()) {
            QString errorMessage = "Error opening file";
            Log().Error(errorMessage);
            emit setTransferError(true, errorMessage);
            return -1;
        }

        file.seekg(0, file.end);
        size_t fileSize = file.tellg();
        file.seekg(0, file.beg);

        char *buffer = new char[fileSize];
        file.read(buffer, fileSize);

        sftp_session sftp = sftp_new(session);
        if (sftp == nullptr) {
            QString errorMessage =
                "Error allocating SFTP session: " + QString::fromStdString(ssh_get_error(session));
            Log().Error(errorMessage);
            emit setTransferError(true, errorMessage);
            return -1;
        }

        rc = sftp_init(sftp);
        if (rc != SSH_OK) {
            QString errorMessage = "Error initializing SFTP session: " +
                                   QString::fromStdString(ssh_get_error(session));
            Log().Error(errorMessage);
            emit setTransferError(true, errorMessage);
            return -1;
        }

        QString fileName = localFile.split("/").last();
        QString remoteFilePath = serverPath + "/" + fileName;

        sftp_file fileHandle =
            sftp_open(sftp, remoteFilePath.toLocal8Bit(), O_WRONLY | O_CREAT | O_TRUNC, S_IRWXU);
        if (fileHandle == nullptr) {
            QString errorMessage = "Error opening file on remote server: " +
                                   QString::fromStdString(ssh_get_error(session));
            Log().Error(errorMessage);
            emit setTransferError(true, errorMessage);
            return -1;
        }

        rc = sftp_write(fileHandle, buffer, fileSize);
        if (rc < 0) {
            QString errorMessage = "Error writing file to remote server: " +
                                   QString::fromStdString(ssh_get_error(session));
            Log().Error(errorMessage);
            emit setTransferError(true, errorMessage);
            return -1;
        }

        Log().Info("File transfer complete");

        sftp_close(fileHandle);
        sftp_free(sftp);
        ssh_disconnect(session);
        ssh_free(session);

        emit setTransferProgress(false);
        return 0;
    });

    return 0;
}

// Oğuz

int Backend::returnGlobalModuleId() { return globalModuleId; }
QString Backend::returnGlobalRegId() { return globalRegId; }
QString Backend::returnGlobalFieldId() { return globalFieldId; }
QString Backend::returnGlobalConfigId() { return QString::number(globalConfigId); }
void Backend::setDefaultConfigId(QString configName) {
    globalConfigId = 0;
    QList<QString> configList = Backend::getConfFileList();
    for (int i = 0; i < configList.length(); i++) {
        if (configList[i] == configName) {
            globalConfigId = i;
            break;
        }
    }
}
void Backend::setGlobalModuleId(int moduleId) { globalModuleId = moduleId; Backend::setFilePath(moduleId);}
void Backend::setGlobalRegId(int regId) { globalRegId = QString::number(regId); }
void Backend::setGlobalFieldId(int fieldId) { globalFieldId = QString::number(fieldId); }

QList<QString> Backend::getFileList() {
    QDir directory(Path::getInstance().getSetupDir() + "/Scoc3/Registers/");
    QStringList yamlFiles = directory.entryList(QStringList() << "*.yaml", QDir::Files);

    return yamlFiles;
}

QList<QString> Backend::getConfFileList() {
    QDir directory(Path::getInstance().getSetupDir() + "/Scoc3/SavedConfigs/");
    QStringList yamlFiles = directory.entryList(QStringList() << "*.yaml", QDir::Files);

    return yamlFiles;
}

void Backend::setFilePath(int moduleId) {
    globalModuleId = moduleId;
    filePath = Path::getInstance().getSetupDir().toStdString() + "/Scoc3/Registers/" + getFileList()[moduleId].toStdString();
}

void Backend::setConfFilePath(int configId) {
    globalConfigId = configId;
    configFilePath = Path::getInstance().getSetupDir().toStdString() + "/Scoc3/SavedConfigs/" + getConfFileList()[configId].toStdString();
}

int Backend::returnSelectedConfigId() { return globalConfigId; }

void Backend::resetConfigId() { globalConfigId = -1; }

QList<QString> Backend::getRegisterList() {
    return vectorToQList(Yaml::getValueList(filePath, "RegName"));
}

QList<QString> Backend::getFieldList(QString regId) {
    if (regId != NULL) {
        globalRegId = regId;
    }

    std::vector<YAML::Node> nodeList = Yaml::getNodeListByKey(filePath, "Fields");
    return vectorToQList(Yaml::getValueList(nodeList.at(regId.toInt()), "Name"));
}

int Backend::getConfType(QString fieldId) {
    globalFieldId = fieldId;
    std::vector<YAML::Node> nodeList = Yaml::getNodeListByKey(filePath, "Fields");

    return vectorToQList(Yaml::getValueList(nodeList.at(globalRegId.toInt()), "ConfType"))
        .at(fieldId.toInt())
        .toInt();
}

int Backend::getReadable(QString fieldId) {
    globalFieldId = fieldId;
    std::vector<YAML::Node> nodeList = Yaml::getNodeListByKey(filePath, "Fields");
    return vectorToQList(Yaml::getValueList(nodeList.at(globalRegId.toInt()), "Read"))
        .at(fieldId.toInt())
        .toInt();
}

int Backend::getWriteable(QString fieldId) {
    globalFieldId = fieldId;
    std::vector<YAML::Node> nodeList = Yaml::getNodeListByKey(filePath, "Fields");
    return vectorToQList(Yaml::getValueList(nodeList.at(globalRegId.toInt()), "Write"))
        .at(fieldId.toInt())
        .toInt();
}

QList<QString> Backend::getValueDescriptions(QString fieldId) {
    globalFieldId = fieldId;
    std::vector<YAML::Node> regNodeList = Yaml::getNodeListByKey(filePath, "Fields");
    YAML::Node regNode = regNodeList.at(globalRegId.toInt());
    std::string regName = Yaml::getValue(regNode, "RegName");

    std::vector<YAML::Node> fieldNodeList = Yaml::searchNodeByKey(regNode, "Name");
    YAML::Node fieldNode = fieldNodeList.at(globalFieldId.toInt());
    std::string fieldName = Yaml::getValue(fieldNode, "Name");

    std::string valueNodePath = regName + ".Fields." + fieldName + ".Value";

    YAML::Node valueNode = Yaml::getNodeByPath(filePath, valueNodePath);
    std::vector<std::string> result = valueNode.as<std::vector<std::string>>();

    return vectorToQList(result);
}

QString Backend::getResetValue(QString fieldId) {
    globalFieldId = fieldId;
    std::vector<YAML::Node> nodeList = Yaml::getNodeListByKey(filePath, "Fields");
    return vectorToQList(Yaml::getValueList(nodeList.at(globalRegId.toInt()), "ResetValue"))
        .at(fieldId.toInt());
}

QString Backend::getFieldAddr() {
    std::string moduleAddr = Yaml::getValue(filePath, "Module_ADDR");
    QString regName = Backend::getRegisterList().at(globalRegId.toInt());
    QString regAddr =
        Backend::vectorToQList(Yaml::getValueList(filePath, "ADDR")).at(globalRegId.toInt());
    std::vector<YAML::Node> nodeList = Yaml::getNodeListByKey(filePath, "Fields");
    std::string fieldRange =
        vectorToQList(Yaml::getValueList(nodeList.at(globalRegId.toInt()), "Range"))
            .at(globalFieldId.toInt())
            .toStdString();

    int moduleAddrInt = std::stoi(moduleAddr, 0, 16);
    int regAddrInt = std::stoi(regAddr.toStdString(), 0, 16);
    int fieldRangeStart = getRangeStart(fieldRange);
    int sum = moduleAddrInt + regAddrInt + fieldRangeStart;

    std::stringstream temp;
    temp << std::hex << sum;

    QString sumStr = QString::fromStdString("0x" + temp.str());
    return sumStr;
}

int Backend::getRangeStart(std::string str) {
    for (int i = str.length(); i >= 0; --i) {
        if (str[i] == ',') {
            str.erase(i);
        }
    }
    str.erase(0, 1);
    std::stoi(str);
    return std::stoi(str);
    ;
}

void Backend::saveConfig(QString writeValue, int base) {
    QString writeValueHex;
    if (base == 10) {
        writeValueHex = "0x" + QString::number(writeValue.toInt(), 16);
    } else if (base == 16) {
        writeValueHex = writeValue;
    }

    std::string moduleName = Backend::getFileList().at(globalModuleId).toStdString();

    for (int i = moduleName.length(); i >= 0; --i) {
        if (moduleName[i] == '.') {
            moduleName.erase(i);
        }
    }

    std::string regName = Backend::getRegisterList().at(globalRegId.toInt()).toStdString();
    std::string fieldName =
        Backend::getFieldList(globalRegId).at(globalFieldId.toInt()).toStdString();

    TreeNode root = parseConfig(configFilePath);

    bool found = false;
    for (int moduleNo = 0; moduleNo < root.children.size(); moduleNo++) {
        TreeNode *module = &root.children.at(moduleNo);
        if (module->name == moduleName) {
            for (int regNo = 0; regNo < module->children.size(); regNo++) {
                TreeNode *reg = &module->children.at(regNo);
                if (reg->name == regName) {
                    for (int fieldNo = 0; fieldNo < reg->children.size(); fieldNo++) {
                        TreeNode *field = &reg->children.at(fieldNo);
                        if (field->name == fieldName) {
                            if (writeValue != "-1") {
                                field->value = writeValueHex.toStdString();
                            }

                            found = true;
                            break;
                        }
                    }
                    if (!found) {
                        if (writeValue != "-1") {
                            TreeNode newField(reg, fieldName, 3, writeValueHex.toStdString());
                            reg->addChild(newField);
                        }
                    }
                    break;
                }
            }
            break;
        }
    }

    ofstream outfile;
    outfile.open(configFilePath);
    bool is_firstLine = true;

    foreach (TreeNode module, root.children) {
        if (is_firstLine) {
            is_firstLine = false;
        } else {
            outfile << endl;
        }

        outfile << module.name + ':' << endl;
        foreach (TreeNode reg, module.children) {
            outfile << ' ' + reg.name + ':' << endl;
            foreach (TreeNode field, reg.children) {
                outfile << "  - " + field.name + ": " << field.value << endl;
            }
        }
    }

    outfile.close();
}

TreeNode Backend::parseConfig(std::string configFilePath) {
    // READ_FILE
    ifstream infile;
    infile.open(configFilePath);
    std::vector<std::string> lines;
    std::string buffer;

    while (getline(infile, buffer)) {
        lines.push_back(buffer);
    }

    infile.close();

    // INSERT INTO TREE
    TreeNode root("ROOT", -1);
    TreeNode *modulePtrHolder;
    TreeNode *regPtrHolder;

    foreach (std::string line, lines) {
        if (line == "") {
            continue;
        }
        int degree = Backend::countSpaces(line);

        // MODULE INSERT
        if (degree == 0) {
            std::string name = Backend::deleteNonAlphaNumerical(line);
            TreeNode newNode(&root, name, degree);
            root.addChild(newNode);
            modulePtrHolder = &root.children.at(root.children.size() - 1);

        }

        // REGISTER INSERT
        else if (degree == 1) {
            std::string name = Backend::deleteNonAlphaNumerical(line);
            TreeNode newNode(modulePtrHolder, name, degree);
            modulePtrHolder->addChild(newNode);
            regPtrHolder = &modulePtrHolder->children.at((modulePtrHolder->children.size()) - 1);
        }

        // FIELD INSERT
        else if (degree == 2) {
            std::string name = Backend::deleteNonAlphaNumerical_Field(line).at(0);
            std::string value = Backend::deleteNonAlphaNumerical_Field(line).at(1);
            TreeNode newNode(regPtrHolder, name, degree, value);
            regPtrHolder->addChild(newNode);
        }
    }

    // RETURN ROOT NODE OF THE TREE
    return root;
}

QString Backend::getValueFromConfigFile() {
    TreeNode root = parseConfig(configFilePath);
    std::string moduleName = Backend::getFileList().at(globalModuleId).toStdString();

    for (int i = 0; i < 5; ++i) {
        moduleName.pop_back();
    }
    std::string regName = Backend::getRegisterList().at(globalRegId.toInt()).toStdString();
    std::string fieldName =
        Backend::getFieldList(globalRegId).at(globalFieldId.toInt()).toStdString();
    foreach (TreeNode module, root.children) {
        if (module.name == moduleName) {
            foreach (TreeNode reg, module.children) {
                if (reg.name == regName) {
                    foreach (TreeNode field, reg.children) {
                        if (field.name == fieldName) {
                            return QString::fromStdString(field.value);
                        }
                    }
                }
            }
        }
    }

    return "-1";
}

std::string Backend::getFieldAddrByPath(std::string path) {
    std::string moduleName;
    std::string regName;
    std::string fieldName;

    // SPLIT PATH START
    int nameSwitch = 0;
    for (int i = 0; i < path.size(); i++) {
        if (path[i] != '.') {
            switch (nameSwitch) {
            case 0:
                moduleName.push_back(path[i]);
                break;
            case 1:
                regName.push_back(path[i]);
                break;
            case 2:
                fieldName.push_back(path[i]);
                break;
            }
        } else {
            nameSwitch++;
        }
    }
    // SPLIT PATH END

    // GET COMPONENT IDs START
    int moduleId = -1;
    QList<QString> fileList = Backend::getFileList();
    for (int i = 0; i < fileList.size(); i++) {
        std::string temp = fileList.at(i).toStdString();
        for (int j = 0; j < 5; ++j) {
            temp.pop_back();
        }
        if (moduleName == temp) {
            moduleId = i;
            break;
        }
    }

    int regId = -1;
    QList<QString> regList = vectorToQList(
        Yaml::getValueList(Path::getInstance().getSetupDir().toStdString() + "/Scoc3/Registers/" + getFileList()[moduleId].toStdString(), "RegName"));
    for (int i = 0; i < regList.size(); i++) {
        if (regList.at(i).toStdString() == regName) {
            regId = i;
        }
    }

    int fieldId = -1;
    QList<QString> fieldList;
    std::vector<YAML::Node> nodeList =
        Yaml::getNodeListByKey(Path::getInstance().getSetupDir().toStdString() + "/Scoc3/Registers/" + getFileList()[moduleId].toStdString(), "Fields");
    fieldList = vectorToQList(Yaml::getValueList(nodeList.at(regId), "Name"));
    for (int i = 0; i < fieldList.size(); i++) {
        if (fieldList.at(i).toStdString() == fieldName) {
            fieldId = i;
        }
    }

    if (moduleId == -1) {
        qDebug() << "INVALID MODULE on function Backend::getFieldAddrByPath()";
    }
    if (regId == -1) {
        qDebug() << "INVALID REGISTER on function Backend::getFieldAddrByPath()";
    }
    if (fieldId == -1) {
        qDebug() << "INVALID FIELD on function Backend::getFieldAddrByPath()";
    }
    // GET COMPONENT IDs END

    // CALCULATE ADDR START
    std::string moduleAddr =
        Yaml::getValue(Path::getInstance().getSetupDir().toStdString() + "/Scoc3/Registers/" + getFileList()[moduleId].toStdString(), "Module_ADDR");
    QString regAddr =
        Backend::vectorToQList(
            Yaml::getValueList(Path::getInstance().getSetupDir().toStdString() + "/Scoc3/Registers/" + getFileList()[moduleId].toStdString(), "ADDR"))
            .at(regId);
    std::string fieldRange =
        vectorToQList(Yaml::getValueList(nodeList.at(regId), "Range")).at(fieldId).toStdString();

    int moduleAddrInt = std::stoi(moduleAddr, 0, 16);
    int regAddrInt = std::stoi(regAddr.toStdString(), 0, 16);
    int fieldRangeStart = getRangeStart(fieldRange);
    int sum = moduleAddrInt + regAddrInt + fieldRangeStart;
    // CALCULATE ADDR END

    std::stringstream temp;
    temp << std::hex << sum;

    return ("0x" + temp.str());
}

bool Backend::getIsFieldWriteOnlyByPath(std::string path) {
    std::string moduleName;
    std::string regName;
    std::string fieldName;

    // SPLIT PATH START
    int nameSwitch = 0;
    for (int i = 0; i < path.size(); i++) {
        if (path[i] != '.') {
            switch (nameSwitch) {
            case 0:
                moduleName.push_back(path[i]);
                break;
            case 1:
                regName.push_back(path[i]);
                break;
            case 2:
                fieldName.push_back(path[i]);
                break;
            }
        } else {
            nameSwitch++;
        }
    }
    // SPLIT PATH END

    // GET COMPONENT IDs START
    int moduleId = -1;
    QList<QString> fileList = Backend::getFileList();
    for (int i = 0; i < fileList.size(); i++) {
        std::string temp = fileList.at(i).toStdString();
        for (int j = 0; j < 5; ++j) {
            temp.pop_back();
        }
        if (moduleName == temp) {
            moduleId = i;
            break;
        }
    }

    int regId = -1;
    QList<QString> regList = vectorToQList(
        Yaml::getValueList(Path::getInstance().getSetupDir().toStdString() + "/Scoc3/Registers/" + getFileList()[moduleId].toStdString(), "RegName"));
    for (int i = 0; i < regList.size(); i++) {
        if (regList.at(i).toStdString() == regName) {
            regId = i;
        }
    }

    int fieldId = -1;
    QList<QString> fieldList;
    std::vector<YAML::Node> nodeList =
        Yaml::getNodeListByKey(Path::getInstance().getSetupDir().toStdString() + "/Scoc3/Registers/" + getFileList()[moduleId].toStdString(), "Fields");
    fieldList = vectorToQList(Yaml::getValueList(nodeList.at(regId), "Name"));
    for (int i = 0; i < fieldList.size(); i++) {
        if (fieldList.at(i).toStdString() == fieldName) {
            fieldId = i;
        }
    }

    if (moduleId == -1) {
        qDebug() << "INVALID MODULE on function Backend::getFieldWriteableByPath()";
    }
    if (regId == -1) {
        qDebug() << "INVALID REGISTER on function Backend::getFieldWriteableByPath()";
    }
    if (fieldId == -1) {
        qDebug() << "INVALID FIELD on function Backend::getFieldWriteableByPath()";
    }
    // GET COMPONENT IDs END

    // GET IS READ-WRITEABLE START
    nodeList =
        Yaml::getNodeListByKey(Path::getInstance().getSetupDir().toStdString() + "/Scoc3/Registers/" + getFileList()[moduleId].toStdString(), "Fields");
    bool is_writeable =
        vectorToQList(Yaml::getValueList(nodeList.at(regId), "Write")).at(fieldId).toInt();
    bool is_readable =
        vectorToQList(Yaml::getValueList(nodeList.at(regId), "Read")).at(fieldId).toInt();
    // GET IS READ-WRITEABLE END

    return (is_writeable && !is_readable);
}

int Backend::checkAllConfigValues(int mode, QString check) {
    static std::vector<std::string> redModules;
    static std::vector<std::string> redRegs;
    static std::vector<std::string> redFields;

    if (mode == -1) {
        TreeNode root = parseConfig(configFilePath);

        redModules.clear();
        redRegs.clear();
        redFields.clear();

        for (int moduleIt = 0; moduleIt < root.children.size(); moduleIt++) {
            for (int regIt = 0; regIt < root.children.at(moduleIt).children.size(); regIt++) {
                for (int fieldIt = 0;
                     fieldIt < root.children.at(moduleIt).children.at(regIt).children.size();
                     ++fieldIt) {
                    std::string moduleName = root.children.at(moduleIt).name;
                    std::string regName = root.children.at(moduleIt).children.at(regIt).name;
                    std::string fieldName =
                        root.children.at(moduleIt).children.at(regIt).children.at(fieldIt).name;
                    std::string fieldValue =
                        root.children.at(moduleIt).children.at(regIt).children.at(fieldIt).value;
                    if (getIsFieldWriteOnlyByPath(moduleName + '.' + regName + '.' + fieldName)) {
                        continue;
                    } else if (QString::fromStdString(fieldValue) !=
                               Backend::sshGet(QString::fromStdString(getFieldAddrByPath(
                                   moduleName + '.' + regName + '.' + fieldName)))) {
                        redFields.push_back(moduleName + '.' + regName + '.' + fieldName);

                        bool isAvailable = false;
                        foreach (std::string reg, redRegs) {
                            if (reg == (moduleName + '.' + regName)) {
                                isAvailable = true;
                                break;
                            }
                        }

                        if (!isAvailable) {
                            redRegs.push_back((moduleName + '.' + regName));
                        }

                        isAvailable = false;

                        foreach (std::string mod, redModules) {
                            if (mod == moduleName) {
                                isAvailable = true;
                                break;
                            }
                        }

                        if (!isAvailable) {
                            redModules.push_back(moduleName);
                        }
                    }
                }
            }
        }
        return -1;
    }

    else if (mode == 0) {
        foreach (QString module, Backend::vectorToQList(redModules)) {
            if (module == check) {
                return 1;
            }
        }
        return 0;
    }

    else if (mode == 1) {
        foreach (QString reg, Backend::vectorToQList(redRegs)) {
            if (reg == check) {
                return 1;
            }
        }
        return 0;
    }

    else if (mode == 2) {
        foreach (QString field, Backend::vectorToQList(redFields)) {
            if (field == check) {
                return 1;
            }
        }
        return 0;
    }

    else {
        return -1;
    }
}

QString Backend::returnHex(QString num) { return "0x" + QString::number(num.toInt(), 16); }

int Backend::countSpaces(std::string data) {
    int counter = 0;
    foreach (char character, data) {
        if (character == ' ') {
            counter++;
        } else {
            break;
        }
    }
    return counter;
}

void Backend::sshSet(QString address, QString value) {
    ifstream infile;
    infile.open(Path::getInstance().getSetupDir().toStdString() + "/Scoc3/TargetMocks/target.yaml");
    std::vector<std::string> lines;
    std::string buffer;

    while (std::getline(infile, buffer)) {
        lines.push_back(buffer);
    }

    infile.close();
    int i;
    std::string temp;
    bool found = false;

    for (i = 0; i < lines.size(); i++) {
        std::string line = lines.at(i);
        temp.clear();
        for (int j = 0; j < line.size(); j++) {
            if (line.at(j) == ':') {
                break;
            }
            temp.push_back(line[j]);
        }

        if (temp == address.toStdString()) {
            found = true;
            break;
        }
    }

    if (found) {
        lines.at(i) = temp + ": " + value.toStdString();
    }

    else {
        lines.push_back(address.toStdString() + ": " + value.toStdString());
    }

    ofstream outfile;
    outfile.open(Path::getInstance().getSetupDir().toStdString() + "/Scoc3/TargetMocks/target.yaml");

    foreach (std::string line, lines) {
        outfile << line << endl;
    }

    outfile.close();
}

QString Backend::sshGet(QString address) {
    ifstream infile;
    infile.open(Path::getInstance().getSetupDir().toStdString() + "/Scoc3/TargetMocks/target.yaml");
    std::string buffer;

    while (std::getline(infile, buffer)) {
        std::string temp;
        int i;
        for (i = 0; i < buffer.size(); i++) {
            char letter = buffer.at(i);
            if (letter == ':') {
                break;
            }
            temp.push_back(letter);
        }
        if (temp == address.toStdString()) {
            buffer.erase(0, (i + 2));
            infile.close();
            return QString::fromStdString(buffer);
        }
    }

    infile.close();
    return "NULL";
}

void Backend::checkAndSaveAll(QString newFileName) {
    // READ_FILE
    ifstream infile;
    infile.open(Path::getInstance().getSetupDir().toStdString() + "/Scoc3/config.yaml");
    std::vector<std::string> lines;
    std::string buffer;

    while (getline(infile, buffer)) {
        lines.push_back(buffer);
    }

    infile.close();
    // WRITE_FILE
    ofstream outfile;
    outfile.open(Path::getInstance().getSetupDir().toStdString() + "/Scoc3/SavedConfigs/" + newFileName.toStdString());
    bool is_firstLine = true;

    foreach (std::string line, lines) {
        outfile << line << endl;
    }

    outfile.close();

    int tempModuleId = globalModuleId;
    QString tempRegId = globalRegId;
    QString tempFieldId = globalFieldId;
    std::string tempConfigFilePath = configFilePath;

    configFilePath = Path::getInstance().getSetupDir().toStdString() + "/Scoc3/SavedConfigs/" + newFileName.toStdString();

    QList<QString> moduleList = Backend::getFileList();
    for (int i = 0; i < moduleList.length(); i++) {
        globalModuleId = i;
        Backend::setFilePath(globalModuleId);
        QList<QString> regList = Backend::getRegisterList();
        for (int j = 0; j < regList.length(); j++) {
            globalRegId = QString::number(j);
            QList<QString> fieldList = Backend::getFieldList(globalRegId);
            for (int k = 0; k < fieldList.length(); k++) {
                globalFieldId = QString::number(k);

                QString value = sshGet(getFieldAddr());

                if (value != "NULL") {
                    qDebug() << moduleList[i] << regList[j] << fieldList[k] << globalModuleId
                             << globalRegId << globalFieldId << getFieldAddr() << value;
                    saveConfig(value, 16);
                }
            }
        }
    }

    globalModuleId = tempModuleId;
    globalRegId = tempRegId;
    globalFieldId = tempFieldId;
    configFilePath = tempConfigFilePath;

    Backend::setDefaultConfigId(newFileName);
}

std::string Backend::deleteNonAlphaNumerical(std::string data) {
    std::string newData;
    foreach (char character, data) {
        if (!((character == ' ') || (character == ':'))) {
            newData.push_back(character);
        }
    }
    return newData;
}

std::vector<std::string> Backend::deleteNonAlphaNumerical_Field(std::string data) {
    std::vector<std::string> newData;
    newData.push_back("");
    newData.push_back("");

    int switchKeyValue = 0;
    foreach (char character, data) {
        if (character != ' ') {
            if (character == ':') {
                switchKeyValue = 1;
            } else {
                newData.at(switchKeyValue).push_back(character);
            }
        }
    }

    if (newData.at(0).at(0) == '-') {
        newData.at(0).erase(0, 1);
    }

    return newData;
}

int Backend::searchNodeVector(std::vector<TreeNode> container, std::string key) {
    for (int i = 0; i < container.size(); ++i) {
        if (container.at(i).name == key) {
            return i;
        }
    }
    return -1;
}

int Backend::getIdByName(std::string component, std::string name) {
    if (component == "module") {
        QList<QString> moduleList = Backend::getFileList();
        for (int i = 0; i < moduleList.length(); ++i) {
            if (moduleList.at(i) == QString::fromStdString((name + ".yaml"))) {
                return i;
            }
        }
    } else if (component == "reg") {
        Backend::setFilePath(globalModuleId);
        QList<QString> regList = Backend::getRegisterList();
        for (int i = 0; i < regList.length(); ++i) {
            if (regList.at(i) == QString::fromStdString(name)) {
                return i;
            }
        }
    } else if (component == "field") {
        QList<QString> fieldList = Backend::getFieldList(globalRegId);
        for (int i = 0; i < fieldList.length(); ++i) {
            if (fieldList.at(i) == QString::fromStdString(name)) {
                return i;
            }
        }
    }

    return -1;
}

QList<QString> Backend::vectorToQList(std::vector<std::string> vector) {
    QList<QString> qlist;

    for (auto item : vector) {
        qlist.append(QString::fromStdString(item));
    }

    return qlist;
}


int Backend::returnPinConfig(QString initSignal) {
    // READ_FILE
    ifstream infile;
    infile.open(Path::getInstance().getSetupDir().toStdString() + "/Scoc3/pinSlots.yaml");
    std::vector<std::string> lines;
    std::string buffer;

    while (getline(infile, buffer)) {
        lines.push_back(buffer);
    }

    infile.close();
    // READ_FILE

    globalPinConfig.clear();

    for (int i=0; i<lines.size(); i++) {
        int dotCounter = 0;
        std::vector<std::string> buffer;
        buffer.push_back("0");
        std::string temp;
        for (int j=2; j<lines.at(i).size(); j++) {

            if (lines.at(i).at(j)=='.') {
                dotCounter++;
                buffer.at(0) = std::to_string(dotCounter);
                buffer.push_back(temp);
                temp.clear();


            }
            else {
                temp.push_back(lines.at(i).at(j));
                if (j==lines.at(i).size()-1) {
                    dotCounter++;
                    buffer.at(0) = std::to_string(dotCounter);
                    buffer.push_back(temp);
                    temp.clear();
                }
            }
        }
        globalPinConfig.push_back(Backend::vectorToQList(buffer));

    }

    return globalPinConfig.size();
}

QList<QString> Backend::returnPinConfig(int index) {
    return globalPinConfig.at(index);
}

void Backend::addToPinConfig(QString componentType, QString componentId) {
    int componentTypeInt;
    if (componentType.toStdString() == "module") {componentTypeInt = 1;}
    else if (componentType.toStdString() == "reg") {componentTypeInt = 2;}
    else if (componentType.toStdString() == "field") {componentTypeInt = 3;}
    else {componentTypeInt = componentType.toInt();}

    std::string componentPath;
    bool found = false;
    switch (componentTypeInt) {
    case 1: {
        std::string tempModuleName = Backend::getFileList().at(componentId.toInt()).toStdString();
        std::string moduleName;
        foreach (char it, tempModuleName) {
            if (it == '.'){break;}
            moduleName.push_back(it);
        }
        componentPath = "- " + moduleName;

        for (int i = 0; i < Backend::returnPinConfig("init"); i++) {
            if(globalPinConfig.at(i).at(0) == "1"){
                if (globalPinConfig.at(i).at(1).split('\r').at(0) == QString::fromStdString(moduleName)){
                    found = true;
                }
            }
        }

        break;
    }
    case 2: {
        std::string tempModuleName = Backend::getFileList().at(globalModuleId).toStdString();
        std::string moduleName;
        foreach (char it, tempModuleName) {
            if (it == '.'){break;}
            moduleName.push_back(it);
        }

        std::string regName = Backend::getRegisterList().at(componentId.toInt()).toStdString();
        componentPath = "- " + moduleName + '.' + regName;

        for (int i = 0; i < Backend::returnPinConfig("init"); i++) {
            if(globalPinConfig.at(i).at(0) == "2"){
                if ((globalPinConfig.at(i).at(1) == QString::fromStdString(moduleName))
                    && (globalPinConfig.at(i).at(2).split('\r').at(0) == QString::fromStdString(regName))){
                    found = true;
                }
            }
        }

        break;
    }
    case 3: {
        std::string tempModuleName = Backend::getFileList().at(globalModuleId).toStdString();
        std::string moduleName;
        foreach (char it, tempModuleName) {
            if (it == '.'){break;}
            moduleName.push_back(it);
        }

        std::string regName = Backend::getRegisterList().at(globalRegId.toInt()).toStdString();

        std::string fieldName = Backend::getFieldList(globalRegId).at(componentId.toInt()).toStdString();

        componentPath = "- " + moduleName + '.' + regName + '.' + fieldName;

        for (int i = 0; i < Backend::returnPinConfig("init"); i++) {
            if(globalPinConfig.at(i).at(0) == "3"){
                if ((globalPinConfig.at(i).at(1) == QString::fromStdString(moduleName))
                    && (globalPinConfig.at(i).at(2) == QString::fromStdString(regName))
                    && (globalPinConfig.at(i).at(3).split('\r').at(0) == QString::fromStdString(fieldName))){
                    found = true;
                }
            }
        }

        break;
    }
    }

    if (!found) {
        qDebug()<< QString::fromStdString(componentPath)<< "WILL BE ADDED TO PIN AREA";
        std::ofstream outFile;
        outFile.open(Path::getInstance().getSetupDir().toStdString() + "/Scoc3/pinSlots.yaml", std::ios::app);
        if (outFile.is_open()) {
            // Write the line at the end of the file
            outFile << componentPath << std::endl; //ENDLINE SILMEYI DENE !!!!!!


            outFile.close();
            qDebug() << "Line added successfully.";
        } else {
            qDebug() << "Failed to open the file.";
        }
    }
    else {
        qDebug()<< "COMPONENT IS ALREADY IN THE PIN LIST";
    }


}






















