#ifndef COMMON_H
#define COMMON_H

#include "lib/YAML/yaml.h"
#include <QtCore/QtCore>
#include <filesystem>

using std::filesystem::current_path;

namespace Path {
const QString project_root = QString::fromStdString(current_path().string()) + "/../../";
const QString paths_Yaml = project_root + "Setup/Paths.yaml";
const QString bin_Dir = project_root + QString::fromStdString(Yaml::getText<std::vector<std::string>>(Yaml::getNodeByTag<std::vector<YAML::Node>>(paths_Yaml.toStdString(), "bin_Dir").at(0), "bin_Dir").at(0));
const QString server_cmds_Yaml = project_root + QString::fromStdString(Yaml::getText<std::vector<std::string>>(Yaml::getNodeByTag<std::vector<YAML::Node>>(paths_Yaml.toStdString(), "server_cmds_Yaml").at(0), "server_cmds_Yaml").at(0));
const QString client_cmds_Yaml = project_root + QString::fromStdString(Yaml::getText<std::vector<std::string>>(Yaml::getNodeByTag<std::vector<YAML::Node>>(paths_Yaml.toStdString(), "client_cmds_Yaml").at(0), "client_cmds_Yaml").at(0));
const QString config_Yaml = project_root + QString::fromStdString(Yaml::getText<std::vector<std::string>>(Yaml::getNodeByTag<std::vector<YAML::Node>>(paths_Yaml.toStdString(), "config_Yaml").at(0), "config_Yaml").at(0));
} // namespace Path
#endif  // COMMON_H
