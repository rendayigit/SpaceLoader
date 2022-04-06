#ifndef COMMON_H
#define COMMON_H

#include <QtCore/QtCore>
#include <filesystem>

#include "lib/YAML/yaml.h"

using std::filesystem::current_path;

namespace Path {
const QString project_root = QString::fromStdString(current_path().string()) + "/../../";

const QString paths_Yaml = project_root + "Setup/Paths.yaml";

const QString bin_Dir =
    project_root + QString::fromStdString(
                       Yaml::getValue(paths_Yaml.toStdString(), "bin_Dir"));

const QString server_cmds_Yaml =
    project_root +
    QString::fromStdString(
        Yaml::getValue(paths_Yaml.toStdString(), "server_cmds_Yaml"));

const QString client_cmds_Yaml =
    project_root +
    QString::fromStdString(
        Yaml::getValue(paths_Yaml.toStdString(), "client_cmds_Yaml"));

const QString config_Yaml =
    project_root +
    QString::fromStdString(
        Yaml::getValue(paths_Yaml.toStdString(), "config_Yaml"));

}  // namespace Path
#endif  // COMMON_H
