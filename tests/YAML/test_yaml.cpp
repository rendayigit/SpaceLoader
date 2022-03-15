#include <gtest/gtest.h>

#include "../../lib/YAML/yaml.h"
#include "../Test_common.h"
#include "yaml-cpp/yaml.h"

using std::string;
using std::vector;
using YAML::Node;

char const *testYamlFile = "../../tests/YAML/Test.yaml";

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
    EXPECT_EQ(node["name"].as<string>(), "Leve4SiblingName");
}

TEST(YamlTest, getNodeListByKey) {
    vector<Node> nodes = Yaml::getNodeListByKey(testYamlFile, "aKey");
    EXPECT_EQ(nodes.at(0).as<int>(), 1234);
    EXPECT_EQ(nodes.at(1).as<int>(), 12345);
    EXPECT_EQ(nodes.at(2).as<int>(), 123456);
    EXPECT_EQ(nodes.size(), 3);
}

TEST(YamlTest, getNodeListByKeyAndValue) {
    vector<Node> nodes = Yaml::getNodeListByKey(testYamlFile, "key", "value");
    EXPECT_EQ(nodes.at(0)["name"].as<string>(), "Leve4SiblingName");
    EXPECT_EQ(nodes.at(1)["name"].as<string>(), "Level2Item1Name");
    // TODO: check difference between usages above and below
    EXPECT_EQ(nodes.at(0).as<string>(), "value");
    EXPECT_EQ(nodes.at(1).as<string>(), "value");
    EXPECT_EQ(nodes.size(), 2);
}

TEST(YamlTest, getNodeByPath) {
    Node node = Yaml::getNodeByPath(testYamlFile, "NestedItems.Level2.Level3.Leve4Sibling");
    EXPECT_EQ(node["name"].as<string>(), "Leve4SiblingName");
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
    Node node = Yaml::getNodeByPath(testYamlFile, "NestedItems.Level2.Level3.Leve4");
    vector<string> values = Yaml::getValueList(node, "text");
    EXPECT_EQ(values.at(0), "Level4Item1");
    EXPECT_EQ(values.at(1), "Level4Item2");
    EXPECT_EQ(values.at(2), "Level4Item3");
    EXPECT_EQ(values.size(), 3);
}

TEST(YamlTest, getValueListViaYamlFile) {
    vector<string> values = Yaml::getValueList(testYamlFile, "aKey");
    EXPECT_EQ(values.at(0), "1234");
    EXPECT_EQ(values.at(1), "12345");
    EXPECT_EQ(values.at(3), "123456");
    EXPECT_EQ(values.size(), 3);
}