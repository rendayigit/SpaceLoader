#ifndef PATH_H
#define PATH_H

#include <QtCore/QCoreApplication>
#include <QtCore/QString>

#include "lib/YAML/yaml.h"

namespace Path {
/**
 * Returns the path of the executable file.
 *
 * @return The path of the executable file.
 */
[[nodiscard]] static QString getExecutablePath() {
    QString bin = QCoreApplication::applicationFilePath();
    int lastSlashIndex = bin.lastIndexOf('/');
    return bin.left(lastSlashIndex + 1);
}

/**
 * Retrieves the root directory of the project.
 *
 * @return The root directory of the project.
 */
[[nodiscard]] static QString getProjectRoot() {
    QString executablePath = getExecutablePath();
    QString projectRoot = executablePath + "../";
    return projectRoot;
}

/**
 * Retrieves the setup directory.
 *
 * @return The setup directory as a QString
 */
[[nodiscard]] static QString getSetupDir() {
    QString executablePath = getExecutablePath();
    QString setupDir = executablePath + "Setup/";
    return setupDir;
}

/**
 * Retrieves the path to the YAML file containing the paths.
 *
 * @return The path to the YAML file.
 */
[[nodiscard]] static QString getPathsYaml() {
    QString setupDir = getSetupDir();
    QString pathsYaml = setupDir + "Paths.yaml";
    return pathsYaml;
}

/**
 * Retrieves the directory path for the commands.
 *
 * @return The directory path for the commands.
 */
[[nodiscard]] static QString getCmdsDir() {
    std::string pathsYaml = getPathsYaml().toStdString();
    std::string cmdLibDir = Yaml::getValue(pathsYaml, "cmd_lib_Dir");
    QString setupDir = getSetupDir();
    QString cmdsDir = setupDir + QString::fromStdString(cmdLibDir);
    return cmdsDir;
}

/**
 * Retrieves the path to the server commands YAML file.
 *
 * @return The path to the server commands YAML file.
 */
[[nodiscard]] static QString getServerCmdsYaml() {
    QString setupDir = getSetupDir();
    std::string pathsYaml = getPathsYaml().toStdString();
    std::string serverCmdsYaml = Yaml::getValue(pathsYaml, "server_cmds_Yaml");
    return setupDir + QString::fromStdString(serverCmdsYaml);
}

/**
 * Retrieves the YAML path for client commands.
 *
 * @return the YAML path for client commands as a QString.
 */
[[nodiscard]] static QString getClientCmdsYaml() {
    std::string clientCmdsYamlPath = getPathsYaml().toStdString();
    std::string clientCmdsYamlKey = "client_cmds_Yaml";
    std::string clientCmdsYamlValue = Yaml::getValue(clientCmdsYamlPath, clientCmdsYamlKey);
    QString setupDir = getSetupDir();
    QString clientCmdsYaml = QString::fromStdString(clientCmdsYamlValue);
    return setupDir + clientCmdsYaml;
}

/**
 * Retrieves the configuration YAML file path.
 *
 * @return the path to the configuration YAML file
 */
[[nodiscard]] static QString getConfigYaml() {
    std::string setupDir = getSetupDir().toStdString();
    std::string pathsYaml = getPathsYaml().toStdString();
    std::string configYaml = Yaml::getValue(pathsYaml, "config_Yaml");
    return QString::fromStdString(setupDir + configYaml);
}
};  // namespace Path

#endif  // PATH_H