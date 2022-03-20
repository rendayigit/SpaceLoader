#include <gtest/gtest.h>

#include "../../lib/YAML/yaml.h"
#include "../Test_common.h"
#include "yaml-cpp/yaml.h"

using std::count;
using std::string;
using std::vector;
using YAML::Node;

char const* testYamlFile = "../../tests/YAML/Test.yaml";

TEST(YamlTest, TestYamlFileCheck) {
    if (access(testYamlFile, F_OK) != 0) {
        GTEST_FAIL() << "cannot find " << testYamlFile;
    }
}

TEST(YamlTest, getNodeByKey) {
    Node node = Yaml::getNodeByKey(testYamlFile, "aKey");
    EXPECT_EQ(node["name"].as<string>(), "node1value");
}

TEST(YamlTest, getNodeByKeyAndValue) {
    Node node = Yaml::getNodeByKey(testYamlFile, "aKey", "123456");
    string resultText = Yaml::getValue(node, "name");

    EXPECT_EQ(resultText, node["name"].as<string>());
    EXPECT_EQ(resultText, "Leve4SiblingName");
}

TEST(YamlTest, getNodeListByKey) {
    vector<Node> nodes = Yaml::getNodeListByKey(testYamlFile, "aKey");
    bool isEqual = false;
    for (const auto& i : nodes) {
        string value = Yaml::getValue(i, "aKey");
        if (value == "1234" or value == "12345" or value == "123456") {
            isEqual = true;
        }
    }
    EXPECT_TRUE(isEqual);

    vector<string> values = {"1234", "12345", "123456"};
    EXPECT_TRUE(count(values.begin(), values.end(), nodes.at(0).as<string>()));
    EXPECT_TRUE(count(values.begin(), values.end(), nodes.at(1).as<string>()));
    EXPECT_TRUE(count(values.begin(), values.end(), nodes.at(2).as<string>()));
}

TEST(YamlTest, getNodeListByKeyAndValue) {
    vector<Node> nodes = Yaml::getNodeListByKey(testYamlFile, "key", "value");
    bool isEqual = true;
    for (const auto& i : nodes) {
        string value = Yaml::getValue(i, "key");
        if (value != "value") {
            isEqual = false;
        }
    }

    // TODO
    // Yaml::getValue(i, "key") vs code below
    // nodes.at(1)["name"].as<string>() vs nodes.at(1).as<string>()

    EXPECT_EQ(isEqual, true);
}

TEST(YamlTest, getNodeByPath) {
    // TODO when we want to get from path, we need node->second but for value we need all node
    Node node = Yaml::getNodeByPath(testYamlFile, "NestedItems.Level2.Level3.Leve4Sibling");
    string resultText = Yaml::getValue(node, "name");

    // TODO
    // Yaml::getValue(node, "name"); vs node["name"].as<string>()
    // TODO getValue(node,..) vs node[".."].as<..>()

    EXPECT_EQ(resultText, "Leve4SiblingName");
}

TEST(YamlTest, getValue) {
    Node node = Yaml::getNodeByPath(testYamlFile, "NestedItems.Level2.Level3.Leve4Sibling");
    string value = Yaml::getValue(node, "name");
    EXPECT_EQ(value, "Leve4SiblingName");
}

TEST(YamlTest, getValueViaYamlFile) {
    string value = Yaml::getValue(testYamlFile, "uniqueKey");
    EXPECT_EQ(value, "unique value");
}

TEST(YamlTest, getValueList) {
    Node node = Yaml::getNodeByPath(testYamlFile, "NestedItems.Level2.Level3.Level4");
    vector<string> values = Yaml::getValueList(node, "text");

    // TODO getValue(node,..) vs node[".."].as<..>()

    EXPECT_TRUE(count(values.begin(), values.end(), "Level4Item1"));
    EXPECT_TRUE(count(values.begin(), values.end(), "Level4Item2"));
    EXPECT_TRUE(count(values.begin(), values.end(), "Level4Item3"));

    EXPECT_EQ(values.size(), 3);
}

TEST(YamlTest, getValueListViaYamlFile) {
    vector<string> values = Yaml::getValueList(testYamlFile, "aKey");

    // TODO getValue(node,..) vs node[".."].as<..>()

    EXPECT_TRUE(count(values.begin(), values.end(), "1234"));
    EXPECT_TRUE(count(values.begin(), values.end(), "12345"));
    EXPECT_TRUE(count(values.begin(), values.end(), "123456"));

    EXPECT_EQ(values.size(), 3);
}