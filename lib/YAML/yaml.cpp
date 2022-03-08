#include "yaml.h"

using namespace std;
// TODO templates return type

template <typename T>
T Yaml::getNodeByTag(const string &yamlFilePath, const string &tagName,
                           const string &tagValue) {
    YAML::Node config = YAML::LoadFile(yamlFilePath);
    vector<YAML::Node> nodes = {};

    return Yaml::searchNodeByTag(config, tagName, tagValue, nodes).at(0);
}

template <typename T>
T Yaml::getNodeListByTag(const string &yamlFilePath, const string &tagName,
                             const string &tagValue) {
    YAML::Node config = YAML::LoadFile(yamlFilePath);
    vector<YAML::Node> nodes = {};

    return Yaml::searchNodeByTag(config, tagName, tagValue, nodes);
}

template <typename T>
T Yaml::getNodeByTag(const string &yamlFilePath, const string &tagName) {
    YAML::Node config = YAML::LoadFile(yamlFilePath);
    vector<YAML::Node> nodes = {};

    return Yaml::searchNodeByTag_(config, tagName, nodes).at(0);
}

template <typename T>
T Yaml::getNodeListByTag(const string &yamlFilePath, const string &tagName) {
    YAML::Node config = YAML::LoadFile(yamlFilePath);
    vector<YAML::Node> nodes = {};

    return Yaml::searchNodeByTag_(config, tagName, nodes);
}

template <typename T>
T Yaml::getText(const YAML::Node &node, const string &tagName) {
    vector<string> texts = {};
    if (node.IsScalar()) {
        return node.as<string>();
    }
    return Yaml::searchText(node, tagName, texts).at(0);
}

template <typename T>
T Yaml::getTextList(const YAML::Node &node, const string &tagName) {
    vector<string> texts = {};
    if (node.IsScalar()) {
        texts.push_back(node.as<string>());
        return texts;
    }
    return Yaml::searchText(node, tagName, texts);
}

template <typename T>
T Yaml::getText(const std::string &yamlFilePath, const std::string &tagName) {
    YAML::Node config = YAML::LoadFile(yamlFilePath);
    vector<string> texts = {};

    return Yaml::searchText(config, tagName, texts).at(0);
}

template <typename T>
T Yaml::getTextList(const std::string &yamlFilePath, const std::string &tagName) {
    YAML::Node config = YAML::LoadFile(yamlFilePath);
    vector<string> texts = {};

    return Yaml::searchText(config, tagName, texts);
}

template <typename T>
T Yaml::getNodeByPath(const string &yamlFilePath, const string &path) {
    YAML::Node config = YAML::LoadFile(yamlFilePath);
    vector<string> pathOrder = Yaml::splitPath(path, '.');
    vector<YAML::Node> nodes = {};

    return Yaml::searchByNodePath(config, pathOrder, 0, nodes).at(0);
}

template <typename T>
T Yaml::getNodeListByPath(const string &yamlFilePath, const string &path) {
    YAML::Node config = YAML::LoadFile(yamlFilePath);
    vector<string> pathOrder = Yaml::splitPath(path, '.');
    vector<YAML::Node> nodes = {};

    return Yaml::searchByNodePath(config, pathOrder, 0, nodes);
}

template <typename T>
T Yaml::getNodeByPath(const string &yamlFilePath, const string &path) {
    YAML::Node config = YAML::LoadFile(yamlFilePath);
    vector<string> pathOrder = Yaml::splitPath(path, '.');
    vector<YAML::Node> nodes = {};

    return Yaml::searchByNodePath(config, pathOrder, 0, nodes).at(0);
}

vector<string> Yaml::splitPath(const string &path, char delimiter) {
    vector<string> result;
    stringstream ss(path);
    string item;

    while (getline(ss, item, delimiter)) {
        result.push_back(item);
    }

    return result;
}

vector<YAML::Node> Yaml::searchByNodePath(const YAML::Node config, vector<string> pathOrder,
                                          int idx, vector<YAML::Node> &nodes) {
    if (idx == pathOrder.size()) {
        return nodes;
    }

    if (config.IsSequence()) {
        for (YAML::const_iterator it = config.begin(); it != config.end(); ++it) {
            YAML::Node temp = *it;
            Yaml::searchByNodePath(temp, pathOrder, idx, nodes);
        }
    } else if (config.IsMap()) {
        for (YAML::const_iterator it = config.begin(); it != config.end(); ++it) {
            if (it->first.as<string>() == pathOrder.at(idx)) {
                if (idx == pathOrder.size() - 1) {
                    nodes.push_back(it->second);
                }
                Yaml::searchByNodePath(it->second, pathOrder, idx + 1, nodes);
            } else {
                Yaml::searchByNodePath(it->second, pathOrder, idx, nodes);
            }
        }
    }
    return nodes;
}

vector<string> Yaml::searchText(const YAML::Node &node, const string &tagName,
                                vector<string> &texts) {
    if (node.IsSequence()) {
        for (YAML::const_iterator it = node.begin(); it != node.end(); ++it) {
            YAML::Node temp = *it;
            Yaml::searchText(temp, tagName, texts);
        }
    } else if (node.IsMap()) {
        for (YAML::const_iterator it = node.begin(); it != node.end(); ++it) {
            if (it->first.as<string>() == tagName) {
                if (it->second.IsScalar()) {
                    texts.push_back(it->second.as<string>());
                }
            } else {
                Yaml::searchText(it->second, tagName, texts);
            }
        }
    }
    return texts;
}

vector<YAML::Node> Yaml::searchNodeByTag(const YAML::Node config, const string &tagName,
                                         const string &tagValue, vector<YAML::Node> &nodes) {
    if (config.IsSequence()) {
        for (YAML::const_iterator it = config.begin(); it != config.end(); ++it) {
            YAML::Node temp = *it;
            Yaml::searchNodeByTag(temp, tagName, tagValue, nodes);
        }
    } else if (config.IsMap()) {
        for (YAML::const_iterator it = config.begin(); it != config.end(); ++it) {
            if (it->first.as<string>() == tagName and it->second.as<string>() == tagValue) {
                nodes.push_back(config);
            } else {
                Yaml::searchNodeByTag(it->second, tagName, tagValue, nodes);
            }
        }
    }
    return nodes;
}

vector<YAML::Node> Yaml::searchNodeByTag_(const YAML::Node config, const string &tagName,
                                          vector<YAML::Node> &nodes) {
    if (config.IsSequence()) {
        for (YAML::const_iterator it = config.begin(); it != config.end(); ++it) {
            YAML::Node temp = *it;
            Yaml::searchNodeByTag_(temp, tagName, nodes);
        }
    } else if (config.IsMap()) {
        for (YAML::const_iterator it = config.begin(); it != config.end(); ++it) {
            if (it->first.as<string>() == tagName) {
                nodes.push_back(it->second);
            } else {
                Yaml::searchNodeByTag_(it->second, tagName, nodes);
            }
        }
    }
    return nodes;
}

template vector<YAML::Node> Yaml::getNodeListByTag<vector<YAML::Node>>(
    const string &yamlFilePath, const string &tagName, const string &tagValue);
template YAML::Node Yaml::getNodeByTag<YAML::Node>(const string &yamlFilePath,
                                                         const string &tagName,
                                                         const string &tagValue);

template vector<YAML::Node> Yaml::getNodeListByTag(const string &yamlFilePath,
                                                       const string &tagName);
template YAML::Node Yaml::getNodeByTag<YAML::Node>(const string &yamlFilePath,
                                                         const string &tagName);

template vector<string> Yaml::getTextList<vector<string>>(const YAML::Node &node,
                                                              const string &tagName);
template string Yaml::getText<string>(const YAML::Node &node, const string &tagName);

template vector<string> Yaml::getTextList<vector<string>>(const string &yamlFilePath,
                                                              const string &tagName);
template string Yaml::getText<string>(const string &yamlFilePath, const string &tagName);

template vector<YAML::Node> Yaml::getNodeListByPath<vector<YAML::Node>>(
    const string &yamlFilePath, const string &path);
template YAML::Node Yaml::getNodeByPath<YAML::Node>(const string &yamlFilePath,
                                                          const string &path);