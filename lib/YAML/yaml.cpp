#include "yaml.h"

#include <vector>

using std::string;
using std::stringstream;
using std::vector;
using YAML::const_iterator;
using YAML::LoadFile;
using YAML::Node;

Node Yaml::getNodeByTag(const string &yamlFilePath, const string &tagName) {
    Node rootNode = LoadFile(yamlFilePath);

    return searchNodeByTag(rootNode, tagName).at(0);
}

Node Yaml::getNodeByTag(const string &yamlFilePath, const string &tagName, const string &tagValue) {
    Node rootNode = LoadFile(yamlFilePath);
    vector<Node> nodeList = {};

    return searchNodeByTag(rootNode, tagName, tagValue).at(0);
}

vector<Node> Yaml::getNodeListByTag(const string &yamlFilePath, const string &tagName) {
    Node rootNode = LoadFile(yamlFilePath);
    vector<Node> nodeList = {};

    return searchNodeByTag(rootNode, tagName);
}

vector<Node> Yaml::getNodeListByTag(const string &yamlFilePath, const string &tagName,
                                    const string &tagValue) {
    Node rootNode = LoadFile(yamlFilePath);
    vector<Node> nodeList = {};

    return searchNodeByTag(rootNode, tagName, tagValue);
}

Node Yaml::getNodeByPath(const string &yamlFilePath, const string &path) {
    Node rootNode = LoadFile(yamlFilePath);
    vector<string> pathOrder = splitPath(path, '.');

    return searchByNodePath(rootNode, pathOrder).at(0);
}

vector<Node> Yaml::getNodeListByPath(const string &yamlFilePath, const string &path) {
    Node rootNode = LoadFile(yamlFilePath);
    vector<string> pathOrder = splitPath(path, '.');

    return searchByNodePath(rootNode, pathOrder);
}

string Yaml::getText(const Node &node, const string &tagName) {
    vector<string> textList = {};

    if (node.IsScalar()) {
        return node.as<string>();
    }

    return searchText(node, tagName).at(0);
}

string Yaml::getText(const std::string &yamlFilePath, const std::string &tagName) {
    Node rootNode = LoadFile(yamlFilePath);
    vector<string> textList = {};

    return searchText(rootNode, tagName).at(0);
}

vector<string> Yaml::getTextList(const Node &node, const string &tagName) {
    vector<string> textList = {};

    if (node.IsScalar()) {
        textList.push_back(node.as<string>());
        return textList;
    }

    return searchText(node, tagName);
}

vector<string> Yaml::getTextList(const std::string &yamlFilePath, const std::string &tagName) {
    Node rootNode = LoadFile(yamlFilePath);
    vector<string> textList = {};

    return searchText(rootNode, tagName);
}

vector<Node> Yaml::searchByNodePath(const Node node, vector<string> pathOrder) {
    vector<Node> resultList;

    if (pathOrder.empty()) {
        return resultList;
    }

    if (node.IsSequence()) {
        for (const_iterator it = node.begin(); it != node.end(); ++it) {
            vector<Node> temp = searchByNodePath(*it, pathOrder);
            for (const Node &i : temp) resultList.push_back(i);
        }
    } else if (node.IsMap()) {
        for (const_iterator it = node.begin(); it != node.end(); ++it) {
            if (it->first.as<string>() == pathOrder.at(0)) {
                if (pathOrder.size() == 1) {
                    resultList.push_back(it->second);
                }
                pathOrder.erase(pathOrder.begin());
                vector<Node> temp = searchByNodePath(it->second, pathOrder);
                for (const Node &i : temp) resultList.push_back(i);
            } else {
                vector<Node> temp = searchByNodePath(it->second, pathOrder);
                for (const Node &i : temp) resultList.push_back(i);
            }
        }
    }
    return resultList;
}

vector<Node> Yaml::searchNodeByTag(const Node node, const string &tagName, const string &tagValue) {
    vector<Node> resultList;

    if (node.IsSequence()) {
        for (const_iterator it = node.begin(); it != node.end(); ++it) {
            vector<Node> temp = searchNodeByTag(*it, tagName, tagValue);
            for (const Node &i : temp) resultList.push_back(i);
        }
    } else if (node.IsMap()) {
        for (const_iterator it = node.begin(); it != node.end(); ++it) {
            if (it->first.as<string>() == tagName and it->second.as<string>() == tagValue) {
                resultList.push_back(node);
            } else {
                vector<Node> temp = searchNodeByTag(it->second, tagName, tagValue);
                for (const Node &i : temp) resultList.push_back(i);
            }
        }
    }

    return resultList;
}

vector<Node> Yaml::searchNodeByTag(const Node node, const string &tagName) {
    vector<Node> resultList;

    if (node.IsSequence()) {
        for (const_iterator it = node.begin(); it != node.end(); ++it) {
            vector<Node> temp = searchNodeByTag(*it, tagName);
            for (const Node &i : temp) resultList.push_back(i);
        }
    } else if (node.IsMap()) {
        for (const_iterator it = node.begin(); it != node.end(); ++it) {
            if (it->first.as<string>() == tagName) {
                resultList.push_back(it->second);
            } else {
                vector<Node> temp = searchNodeByTag(it->second, tagName);
                for (const Node &i : temp) resultList.push_back(i);
            }
        }
    }

    return resultList;
}

vector<string> Yaml::searchText(const Node &node, const string &tagName) {
    vector<string> resultList;

    if (node.IsSequence()) {
        for (const_iterator it = node.begin(); it != node.end(); ++it) {
            vector<string> temp = searchText(*it, tagName);
            for (const string &i : temp) resultList.push_back(i);
        }
    } else if (node.IsMap()) {
        for (const_iterator it = node.begin(); it != node.end(); ++it) {
            if (it->first.as<string>() == tagName) {
                if (it->second.IsScalar()) {
                    resultList.push_back(it->second.as<string>());
                }
            } else {
                vector<string> temp = searchText(it->second, tagName);
                for (const string &i : temp) resultList.push_back(i);
            }
        }
    }

    return resultList;
}

vector<string> Yaml::splitPath(const string &path, char delimiter) {
    vector<string> pathVector;
    stringstream ss(path);
    string item;

    while (getline(ss, item, delimiter)) {
        pathVector.push_back(item);
    }

    return pathVector;
}