#ifndef COMMON_H
#define COMMON_H

#include <QtCore/QDebug>
#include <QtCore/QString>

#include "lib/YAML/yaml.h"

#ifdef Q_OS_WIN
#include <windows.h>
#else
#include <unistd.h>

#include <climits>
#endif

#ifdef Paths
#undef Paths
#endif

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

    QString getBinaryPath() const {
#ifdef Q_OS_WIN
        WCHAR dest[MAX_PATH];
        GetModuleFileNameW(NULL, dest, MAX_PATH);
        QString path = QString::fromStdWString(dest);
        return path.left(path.lastIndexOf(QChar('\\')));
#else
        char dest[PATH_MAX];
        memset(dest, 0, sizeof(dest));
        if (readlink("/proc/self/exe", dest, PATH_MAX) == -1) {
            return "";
        }
        QString path = dest;
        return path.left(path.lastIndexOf(QChar('/')));
#endif
    }

    QString getProjectRoot() const { return getBinaryPath() + "/../../"; }

    QString getPathsYaml() const { return getProjectRoot() + "Setup/Paths.yaml"; }

    QString getBinDir() const {
        return getProjectRoot() +
               QString::fromStdString(Yaml::getValue(getPathsYaml().toStdString(), "bin_Dir"));
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

#endif  // COMMON_H