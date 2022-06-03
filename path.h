#ifndef PATH_H
#define PATH_H

#include <QtCore/QCoreApplication>
#include <QtCore/QDebug>
#include <QtCore/QString>

#include "lib/YAML/yaml.h"

#define Paths Path::getInstance

class Path {
   public:
    Path(const Path &) = delete;
    Path &operator=(const Path &) = delete;
    Path(Path &&) = delete;
    Path &operator=(Path &&) = delete;
    ~Path() = default;

    static auto &getInstance() {
        static Path instance;
        return instance;
    }

    QString getExecutablePath() const {
        QString bin =
            QString::fromStdWString(QCoreApplication::applicationFilePath().toStdWString());
        return bin.mid(0, bin.lastIndexOf(QChar('/')));
    }

    QString getProjectRoot() const { return getExecutablePath() + "/../../"; }

    QString getPathsYaml() const { return getProjectRoot() + "Setup/Paths.yaml"; }

    QString getBinDir() const {
        return getProjectRoot() +
               QString::fromStdString(Yaml::getValue(getPathsYaml().toStdString(), "bin_Dir"));
    }

    QString getCmdsDir() const {
        return getProjectRoot() +
               QString::fromStdString(Yaml::getValue(getPathsYaml().toStdString(), "cmd_lib_Dir"));
    }

    QString getServerCmdsYaml() const {
        return getProjectRoot() + QString::fromStdString(Yaml::getValue(
                                      getPathsYaml().toStdString(), "server_cmds_Yaml"));
    }

    QString getClientCmdsYaml() const {
        return getProjectRoot() + QString::fromStdString(Yaml::getValue(
                                      getPathsYaml().toStdString(), "client_cmds_Yaml"));
    }

    QString getConfigYaml() const {
        return getProjectRoot() +
               QString::fromStdString(Yaml::getValue(getPathsYaml().toStdString(), "config_Yaml"));
    }

   private:
    Path() = default;
};

#endif  // PATH_H