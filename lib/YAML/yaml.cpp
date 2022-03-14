#include "yaml.h"

using std::string;
using std::stringstream;
using std::vector;
using YAML::const_iterator;
using YAML::LoadFile;
using YAML::Node;

Node Yaml::getNodeByKey(const string &yamlFilePath, const string &key) {
    Node rootNode = LoadFile(yamlFilePath);

    return searchNodeByKey(rootNode, key).at(0);
}

Node Yaml::getNodeByKey(const string &yamlFilePath, const string &key, const string &value) {
    Node rootNode = LoadFile(yamlFilePath);

    return searchNodeByKey(rootNode, key, value).at(0);
}

vector<Node> Yaml::getNodeListByKey(const string &yamlFilePath, const string &key) {
    Node rootNode = LoadFile(yamlFilePath);

    return searchNodeByKey(rootNode, key);
}

vector<Node> Yaml::getNodeListByKey(const string &yamlFilePath, const string &key,
                                    const string &value) {
    Node rootNode = LoadFile(yamlFilePath);

    return searchNodeByKey(rootNode, key, value);
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

string Yaml::getText(const Node &node, const string &key) {
    if (node.IsScalar()) {
        return node.as<string>();
    }

    return searchText(node, key).at(0);
}

string Yaml::getText(const std::string &yamlFilePath, const std::string &key) {
    Node rootNode = LoadFile(yamlFilePath);

    return searchText(rootNode, key).at(0);
}

vector<string> Yaml::getTextList(const Node &node, const string &key) {
    vector<string> textList = {};

    if (node.IsScalar()) {
        textList.push_back(node.as<string>());
        return textList;
    }

    return searchText(node, key);
}

vector<string> Yaml::getTextList(const std::string &yamlFilePath, const std::string &key) {
    Node rootNode = LoadFile(yamlFilePath);

    return searchText(rootNode, key);
}

vector<Node> Yaml::searchByNodePath(const Node node, vector<string> pathOrder) {
    if (pathOrder.empty() or node.IsScalar()) {
        return {};
    }

    vector<Node> resultList;

    if (node.IsSequence()) {
        for (const_iterator it = node.begin(); it != node.end(); ++it) {
            vector<Node> temp = searchByNodePath(*it, pathOrder);
            resultList.insert(resultList.end(), temp.begin(), temp.end());
        }
    } else {
        for (const_iterator it = node.begin(); it != node.end(); ++it) {
            if (it->first.as<string>() == pathOrder.at(0)) {
                if (pathOrder.size() == 1) {
                    resultList.push_back(it->second);
                    // TODO: return here?
                }
                pathOrder.erase(pathOrder.begin());
                vector<Node> temp = searchByNodePath(it->second, pathOrder);
                resultList.insert(resultList.end(), temp.begin(), temp.end());
            } else {
                vector<Node> temp = searchByNodePath(it->second, pathOrder);
                resultList.insert(resultList.end(), temp.begin(), temp.end());
            }
        }
    }

    return resultList;
}

vector<Node> Yaml::searchNodeByKey(const Node node, const string &key, const string &value) {
    vector<Node> resultList;

    if (node.IsSequence()) {
        for (const_iterator it = node.begin(); it != node.end(); ++it) {
            vector<Node> temp = searchNodeByKey(*it, key, value);
            resultList.insert(resultList.end(), temp.begin(), temp.end());
        }
    } else if (node.IsMap()) {
        for (const_iterator it = node.begin(); it != node.end(); ++it) {
            if (it->first.as<string>() == key and it->second.as<string>() == value) {
                resultList.push_back(node);
            } else {
                vector<Node> temp = searchNodeByKey(it->second, key, value);
                resultList.insert(resultList.end(), temp.begin(), temp.end());
            }
        }
    }

    return resultList;
}

vector<Node> Yaml::searchNodeByKey(const Node node, const string &key) {
    vector<Node> resultList;

    if (node.IsSequence()) {
        for (const_iterator it = node.begin(); it != node.end(); ++it) {
            vector<Node> temp = searchNodeByKey(*it, key);
            resultList.insert(resultList.end(), temp.begin(), temp.end());
        }
    } else if (node.IsMap()) {
        for (const_iterator it = node.begin(); it != node.end(); ++it) {
            if (it->first.as<string>() == key) {
                resultList.push_back(it->second);
            } else {
                vector<Node> temp = searchNodeByKey(it->second, key);
                resultList.insert(resultList.end(), temp.begin(), temp.end());
            }
        }
    }

    return resultList;
}

vector<string> Yaml::searchText(const Node &node, const string &key) {
    vector<string> resultList;

    if (node.IsSequence()) {
        for (const_iterator it = node.begin(); it != node.end(); ++it) {
            vector<string> temp = searchText(*it, key);
            resultList.insert(resultList.end(), temp.begin(), temp.end());
        }
    } else if (node.IsMap()) {
        for (const_iterator it = node.begin(); it != node.end(); ++it) {
            if (it->first.as<string>() == key and it->second.IsScalar()) {
                resultList.push_back(it->second.as<string>());
            } else {
                vector<string> temp = searchText(it->second, key);
                resultList.insert(resultList.end(), temp.begin(), temp.end());
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