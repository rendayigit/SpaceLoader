#pragma once

#include <yaml-cpp/yaml.h>

#include <iostream>
#include <sstream>

class Yaml {
   public:
    template <typename T>
    static T getNodeByTag(const std::string &yamlFilePath, const std::string &tagName,
                          const std::string &tagValue);

    template <typename T>
    static T getNodeByTag(const std::string &yamlFilePath, const std::string &tagName);

    template <typename T>
    static T getText(const YAML::Node &node, const std::string &tagName);

    template <typename T>
    static T getText(const std::string &yamlFilePath, const std::string &tagName);

    template <typename T>
    static T getNodeByPath(const std::string &yamlFilePath, const std::string &path);

   private:
    Yaml() = default;
    ~Yaml() = default;

    static std::vector<YAML::Node> searchByNodePath(const YAML::Node config, std::vector<std::string> pathOrder, int idx,
                              std::vector<YAML::Node> &snodes);

    static std::vector<std::string> searchText(const YAML::Node &node, const std::string &tagName,
                        std::vector<std::string> &texts);

    static std::vector<YAML::Node> searchNodeByTag(const YAML::Node config, const std::string &tagName,
                             const std::string &tagValue, std::vector<YAML::Node> &nodes);

    static std::vector<YAML::Node> searchNodeByTag_(const YAML::Node config, const std::string &tagName,
                              std::vector<YAML::Node> &nodes);

    static std::vector<std::string> splitPath(const std::string &path, char delimiter);
};