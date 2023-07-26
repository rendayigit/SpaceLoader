#ifndef PATH_H
#define PATH_H

#include <QtCore/QCoreApplication>
#include <QtCore/QString>

#include "lib/YAML/yaml.h"

#define Paths Path::getInstance

class Path {
   public:
    [[nodiscard]] static QString getExecutablePath() {
        QString bin =
            QString::fromStdWString(QCoreApplication::applicationFilePath().toStdWString());
        return bin.mid(0, bin.lastIndexOf(QChar('/')) + 1);
    }

    [[nodiscard]] static QString getProjectRoot() { return getExecutablePath() + "../"; }

    [[nodiscard]] static QString getSetupDir() { return getExecutablePath() + "Setup/"; }

    [[nodiscard]] static QString getPathsYaml() { return getSetupDir() + "Paths.yaml"; }

    [[nodiscard]] static QString getCmdsDir() {
        return getSetupDir() +
               QString::fromStdString(Yaml::getValue(getPathsYaml().toStdString(), "cmd_lib_Dir"));
    }

    [[nodiscard]] static QString getServerCmdsYaml() {
        return getSetupDir() + QString::fromStdString(Yaml::getValue(getPathsYaml().toStdString(),
                                                                     "server_cmds_Yaml"));
    }

    [[nodiscard]] static QString getClientCmdsYaml() {
        return getSetupDir() + QString::fromStdString(Yaml::getValue(getPathsYaml().toStdString(),
                                                                     "client_cmds_Yaml"));
    }

    [[nodiscard]] static QString getConfigYaml() {
        return getSetupDir() +
               QString::fromStdString(Yaml::getValue(getPathsYaml().toStdString(), "config_Yaml"));
    }
};

#endif  // PATH_H