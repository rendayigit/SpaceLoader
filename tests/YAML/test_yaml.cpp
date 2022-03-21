#include <gtest/gtest.h>

#include "../../lib/YAML/yaml.h"
#include "../Test_common.h"
#include "yaml-cpp/yaml.h"

using std::count;
using std::string;
using std::vector;
using YAML::Node;
using YAML::const_iterator;

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
    vector<string> resultTexts = Yaml::getValueList(node, "name");

    EXPECT_EQ("Leve4SiblingName", node["name"].as<string>());

    EXPECT_TRUE(count(resultTexts.begin(), resultTexts.end(), "Level5Item1Name"));
    EXPECT_TRUE(count(resultTexts.begin(), resultTexts.end(), "Leve4SiblingName"));
    EXPECT_TRUE(count(resultTexts.begin(), resultTexts.end(), "Level5Item2Name"));
    EXPECT_TRUE(count(resultTexts.begin(), resultTexts.end(), "Level5Item3Name"));
}

TEST(YamlTest, getNodeListByKey) {
    vector<Node> nodes = Yaml::getNodeListByKey(testYamlFile, "aKey");
    bool valueCheck = false;

    for (const auto& i : nodes) {
        string value = Yaml::getValue(i, "aKey");
        if (value == "1234" or value == "12345" or value == "123456") {
            valueCheck = true;
        }
    }
    EXPECT_TRUE(valueCheck);

    vector<string> values = {"1234", "12345", "123456"};
    EXPECT_TRUE(count(values.begin(), values.end(), nodes.at(0)["aKey"].as<string>()));
    EXPECT_TRUE(count(values.begin(), values.end(), nodes.at(1)["aKey"].as<string>()));
    EXPECT_TRUE(count(values.begin(), values.end(), nodes.at(2)["aKey"].as<string>()));
}

TEST(YamlTest, getNodeListByKeyAndValue) {
    vector<Node> nodes = Yaml::getNodeListByKey(testYamlFile, "key", "value");
    bool valueCheck = true;
    for (const auto& i : nodes) {
        string value = Yaml::getValue(i, "key");
        if (value != "value") {
            valueCheck = false;
        }
    }

    EXPECT_TRUE(valueCheck);
    EXPECT_EQ(nodes.at(0)["key"].as<string>(), "value");
    EXPECT_EQ(nodes.at(1)["key"].as<string>(), "value");

}

TEST(YamlTest, getNodeByPath) {
    Node node = Yaml::getNodeByPath(testYamlFile, "NestedItems.Level2.Level2Item");
    vector<string> resultTexts = Yaml::getValueList(node, "text");

    EXPECT_TRUE(count(resultTexts.begin(), resultTexts.end(), "Level2Item1"));
    EXPECT_TRUE(count(resultTexts.begin(), resultTexts.end(), "Level2Item2"));
    EXPECT_TRUE(count(resultTexts.begin(), resultTexts.end(), "Level2Item3"));
}

TEST(YamlTest, getValue) {
    Node node = Yaml::getNodeByPath(testYamlFile, "NestedItems.Level2.Level3.Leve4Sibling");

    EXPECT_EQ(node["name"].as<string>(), "Leve4SiblingName");
}

TEST(YamlTest, getValueViaYamlFile) {
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