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
    EXPECT_EQ(Yaml::getValue(node, "name"), "node1value");
}

TEST(YamlTest, getNodeByKeyAndValue) {
    Node node = Yaml::getNodeByKey(testYamlFile, "aKey", "123456");
    EXPECT_EQ(node["name"].as<string>(), "Leve4SiblingName");

    vector<string> values = Yaml::getValueList(node, "name");
    EXPECT_TRUE(count(values.begin(), values.end(), "Level5Item1Name"));
    EXPECT_TRUE(count(values.begin(), values.end(), "Leve4SiblingName"));
    EXPECT_TRUE(count(values.begin(), values.end(), "Level5Item2Name"));
    EXPECT_TRUE(count(values.begin(), values.end(), "Level5Item3Name"));
}

TEST(YamlTest, getNodeListByKey) {
    vector<Node> nodes = Yaml::getNodeListByKey(testYamlFile, "aKey");

    vector<string> values = {"1234", "12345", "123456"};

    EXPECT_TRUE(count(values.begin(), values.end(), Yaml::getValue(nodes.at(0), "aKey")));
    EXPECT_TRUE(count(values.begin(), values.end(), Yaml::getValue(nodes.at(1), "aKey")));
    EXPECT_TRUE(count(values.begin(), values.end(), Yaml::getValue(nodes.at(2), "aKey")));

    EXPECT_TRUE(count(values.begin(), values.end(), nodes.at(0)["aKey"].as<string>()));
    EXPECT_TRUE(count(values.begin(), values.end(), nodes.at(1)["aKey"].as<string>()));
    EXPECT_TRUE(count(values.begin(), values.end(), nodes.at(2)["aKey"].as<string>()));

    EXPECT_EQ(nodes.size(), 3);
}

TEST(YamlTest, getNodeListByKeyAndValue) {
    vector<Node> nodes = Yaml::getNodeListByKey(testYamlFile, "key", "value");

    EXPECT_EQ(Yaml::getValue(nodes.at(0), "key"), "value");
    EXPECT_EQ(Yaml::getValue(nodes.at(1), "key"), "value");

    EXPECT_EQ(nodes.at(0)["key"].as<string>(), "value");
    EXPECT_EQ(nodes.at(1)["key"].as<string>(), "value");

    EXPECT_EQ(nodes.size(), 2);
}

TEST(YamlTest, getNodeByPath) {
    Node node = Yaml::getNodeByPath(testYamlFile, "NestedItems.Level2.Level2Item");
    vector<string> values = Yaml::getValueList(node, "text");

    EXPECT_TRUE(count(values.begin(), values.end(), "Level2Item1"));
    EXPECT_TRUE(count(values.begin(), values.end(), "Level2Item2"));
    EXPECT_TRUE(count(values.begin(), values.end(), "Level2Item3"));

    EXPECT_EQ(values.size(), 3);
}

TEST(YamlTest, getValue) {
    Node node = Yaml::getNodeByPath(testYamlFile, "NestedItems.Level2.Level3.Leve4Sibling");
    string value = Yaml::getValue(node, "name");

    EXPECT_EQ(value, "Leve4SiblingName");
    EXPECT_EQ(node["name"].as<string>(), "Leve4SiblingName");
}

TEST(YamlTest, getValueViaYamlFile) {
    Node node = Yaml::getNodeByPath(testYamlFile, "NestedItems.Level2.Level3.Leve4Sibling");
    string value = Yaml::getValue(testYamlFile, "uniqueKey");
    EXPECT_EQ(value, "unique value");
}

TEST(YamlTest, getValueList) {
    Node node = Yaml::getNodeByPath(testYamlFile, "NestedItems.Level2.Level3.Level4");
    vector<string> values = Yaml::getValueList(node, "text");

    EXPECT_TRUE(count(values.begin(), values.end(), "Level4Item1"));
    EXPECT_TRUE(count(values.begin(), values.end(), "Level4Item2"));
    EXPECT_TRUE(count(values.begin(), values.end(), "Level4Item3"));

    EXPECT_EQ(values.size(), 3);
}

TEST(YamlTest, getValueListViaYamlFile) {
    vector<string> values = Yaml::getValueList(testYamlFile, "aKey");

    EXPECT_TRUE(count(values.begin(), values.end(), "1234"));
    EXPECT_TRUE(count(values.begin(), values.end(), "12345"));
    EXPECT_TRUE(count(values.begin(), values.end(), "123456"));

    EXPECT_EQ(values.size(), 3);
}