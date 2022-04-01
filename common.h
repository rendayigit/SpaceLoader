#ifndef COMMON_H
#define COMMON_H


#include <QtCore/QtCore>
#include <iostream>
#include <QProcess>
#include "lib/YAML/yaml.h"
#include <unistd.h>
#include <limits.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

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
 
    QString getBinaryPath() const{ 
        char dest[PATH_MAX];
        memset(dest,0,sizeof(dest)); // readlink does not null terminate!
        if (readlink("/proc/self/exe", dest, PATH_MAX) == -1) {
            perror("readlink");
            return "";
        }
        QString tmp=dest;
        int pos = tmp.lastIndexOf(QChar('/'));
        return tmp.left(pos);
        
    }

    QString getProjectRoot() const { 
        return getBinaryPath() + "/../../";
    }

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
    Path()=default;
    
};

#endif  // COMMON_H