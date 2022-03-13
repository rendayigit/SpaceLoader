#include "yaml.h"

using std::string;
using std::stringstream;
using std::vector;
using YAML::const_iterator;
using YAML::LoadFile;
using YAML::Node;

Node Yaml::getNodeByTag(const string &yamlFilePath, const string &tagName) {
    Node rootNode = LoadFile(yamlFilePath);
    vector<Node> nodeList = {};

    return searchNodeByTag(rootNode, tagName, nodeList).at(0);
}

Node Yaml::getNodeByTag(const string &yamlFilePath, const string &tagName, const string &tagValue) {
    Node rootNode = LoadFile(yamlFilePath);
    vector<Node> nodeList = {};

    return searchNodeByTag(rootNode, tagName, tagValue, nodeList).at(0);
}

vector<Node> Yaml::getNodeListByTag(const string &yamlFilePath, const string &tagName) {
    Node rootNode = LoadFile(yamlFilePath);
    vector<Node> nodeList = {};

    return searchNodeByTag(rootNode, tagName, nodeList);
}

vector<Node> Yaml::getNodeListByTag(const string &yamlFilePath, const string &tagName,
                                    const string &tagValue) {
    Node rootNode = LoadFile(yamlFilePath);
    vector<Node> nodeList = {};

    return searchNodeByTag(rootNode, tagName, tagValue, nodeList);
}

Node Yaml::getNodeByPath(const string &yamlFilePath, const string &path) {
    Node rootNode = LoadFile(yamlFilePath);
    vector<string> pathOrder = splitPath(path, '.');
    vector<Node> nodeList = {};

    return searchByNodePath(rootNode, pathOrder, 0, nodeList).at(0);
}

vector<Node> Yaml::getNodeListByPath(const string &yamlFilePath, const string &path) {
    Node rootNode = LoadFile(yamlFilePath);
    vector<string> pathOrder = splitPath(path, '.');
    vector<Node> nodeList = {};

    return searchByNodePath(rootNode, pathOrder, 0, nodeList);
}

string getText(const Node &node) {
    // TODO implement

    return "";
}

string Yaml::getText(const Node &node, const string &tagName) {
    vector<string> textList = {};

    if (node.IsScalar()) {
        return node.as<string>();
    }

    return searchText(node, tagName, textList).at(0);
}

string getText(const Node &node, const std::string &tagName, const std::string &tagValue) {
    vector<string> textList = {};

    if (node.IsScalar()) {
        return node.as<string>();
    }

    return searchText(node, tagName, tagValue, textList).at(0);
}

string Yaml::getText(const std::string &yamlFilePath, const std::string &tagName) {
    Node rootNode = LoadFile(yamlFilePath);
    vector<string> textList = {};

    return searchText(rootNode, tagName, textList).at(0);
}

string getText(const std::string &yamlFilePath, const std::string &tagName,
               const std::string &tagValue) {
    Node rootNode = LoadFile(yamlFilePath);
    vector<string> textList = {};

    return searchText(rootNode, tagName, tagValue, textList).at(0);
}

vector<string> Yaml::getTextList(const Node &node, const string &tagName) {
    vector<string> textList = {};

    if (node.IsScalar()) {
        textList.push_back(node.as<string>());
        return textList;
    }

    return searchText(node, tagName, textList);
}

vector<string> getTextList(const Node &node, const std::string &tagName,
                           const std::string &tagValue) {
    vector<string> textList = {};

    if (node.IsScalar()) {
        textList.push_back(node.as<string>());
        return textList;
    }

    return searchText(node, tagName, tagValue, textList);
}

vector<string> getTextList(const std::string &yamlFilePath, const std::string &tagName) {
    Node rootNode = LoadFile(yamlFilePath);
    vector<string> textList = {};

    return searchText(rootNode, tagName, textList);
}

vector<string> getTextList(const std::string &yamlFilePath, const std::string &tagName,
                           const std::string &tagValue) {
    Node rootNode = LoadFile(yamlFilePath);
    vector<string> textList = {};

    return searchText(rootNode, tagName, tagValue, textList);
}

vector<Node> Yaml::searchByNodePath(const Node node, vector<string> pathOrder, int idx,
                                    vector<Node> &nodeList) {
    // TODO This algorithm is too hard to read. Instead of doing it this way, simple pop an item
    // from pathOrder each time we recurse. (Basically remove the idx parameter.)

    if (idx == pathOrder.size()) {
        return nodeList;
    }

    if (node.IsSequence()) {
        for (const_iterator it = node.begin(); it != node.end(); ++it) {
            searchByNodePath(*it, pathOrder, idx,
                             nodeList);  // TODO what happens to returned list ?
        }
    } else if (node.IsMap()) {
        for (const_iterator it = node.begin(); it != node.end(); ++it) {
            if (it->first.as<string>() == pathOrder.at(idx)) {
                if (idx == pathOrder.size() - 1) {
                    nodeList.push_back(it->second);
                }
                searchByNodePath(it->second, pathOrder, idx + 1,
                                 nodeList);  // TODO what happens to returned list ?
            } else {
                searchByNodePath(it->second, pathOrder, idx,
                                 nodeList);  // TODO what happens to returned list ?
            }
        }
    }  // TODO check for scalar necessary ?

    return nodeList;
}

vector<Node> Yaml::searchNodeByTag(const Node node, const string &tagName, const string &tagValue,
                                   vector<Node> &nodeList) {
    vector<Node> resultList;
    resultList.insert(resultList.end(), nodeList.begin(), nodeList.end());

    if (node.IsSequence()) {
        for (const_iterator it = node.begin(); it != node.end(); ++it) {
            vector<Node> temp = searchNodeByTag(*it, tagName, tagValue, resultList);
            resultList.insert(resultList.end(), temp.begin(), temp.end());
        }
    } else if (node.IsMap()) {
        for (const_iterator it = node.begin(); it != node.end(); ++it) {
            if (it->first.as<string>() == tagName and it->second.as<string>() == tagValue) {
                resultList.push_back(node);
            } else {
                vector<Node> temp = searchNodeByTag(it->second, tagName, tagValue, resultList);
                resultList.insert(resultList.end(), temp.begin(), temp.end());
            }
        }
    } else if (node.IsScalar()) {
        // TODO: shouldn't we check for this?
        resultList.push_back(node.as<Node>());
    }

    return resultList;
}

vector<Node> Yaml::searchNodeByTag(const Node node, const string &tagName, vector<Node> &nodeList) {
    vector<Node> resultList;
    resultList.insert(resultList.end(), nodeList.begin(), nodeList.end());

    if (node.IsSequence()) {
        for (const_iterator it = node.begin(); it != node.end(); ++it) {
            vector<Node> temp = searchNodeByTag(*it, tagName, resultList);
            resultList.insert(resultList.end(), temp.begin(), temp.end());
        }
    } else if (node.IsMap()) {
        for (const_iterator it = node.begin(); it != node.end(); ++it) {
            if (it->first.as<string>() == tagName) {
                resultList.push_back(it->second);
            } else {
                vector<Node> temp = searchNodeByTag(it->second, tagName, resultList);
                resultList.insert(resultList.end(), temp.begin(), temp.end());
            }
        }
    } else if (node.IsScalar()) {
        // TODO: shouldn't we check for this?
        resultList.push_back(node.as<Node>());
    }

    return resultList;
}

vector<string> Yaml::searchText(const Node &node, const string &tagName, vector<string> &textList) {
    vector<string> resultList;
    resultList.insert(resultList.end(), textList.begin(), textList.end());

    if (node.IsSequence()) {
        for (const_iterator it = node.begin(); it != node.end(); ++it) {
            vector<string> temp = searchText(*it, tagName, resultList);
            resultList.insert(resultList.end(), temp.begin(), temp.end());
        }
    } else if (node.IsMap()) {
        for (const_iterator it = node.begin(); it != node.end(); ++it) {
            if (it->first.as<string>() == tagName) {
                if (it->second.IsScalar()) {
                    resultList.push_back(it->second.as<string>());
                }
            } else {
                vector<string> temp = searchText(it->second, tagName, resultList);
                resultList.insert(resultList.end(), temp.begin(), temp.end());
            }
        }
    } else if (node.IsScalar()) {
        // TODO: shouldn't we check for this?
        resultList.push_back(node.as<string>());
    }

    return resultList;
}

vector<string> Yaml::searchText(const Node &node, const string &tagName, const string &tagValue,
                                vector<string> &textList) {
    // TODO implement

    return textList;
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