#include <gtest/gtest.h>

#include "../../lib/YAML/yaml.h"
#include "../Test_common.h"
#include "yaml-cpp/yaml.h"

using std::string;
using std::vector;
using YAML::Node;
using std::count;
using YAML::const_iterator;

string const testYamlFile = "../../tests/YAML/Test.yaml";

// TEST(YamlTest, TestYamlFileCheck) {
//     if (access(testYamlFile, F_OK) != 0) {
//         GTEST_FAIL() << "cannot find " << testYamlFile;
//     }
// }

TEST(YamlTest, getNodeByKey) {
    vector<Node> resultNodes = Yaml::getNodeListByKey(testYamlFile, "aKey");
    bool isContain = false;
    for(auto i: resultNodes) {
        vector<string> resultText = Yaml::getValueList(i, "name");
        if(std::count(resultText.begin(), resultText.end(), "node1value")) isContain = true;
    }
    EXPECT_EQ(isContain, true);
}

TEST(YamlTest, getNodeByKeyAndValue) {
    Node node = Yaml::getNodeByKey(testYamlFile, "aKey", "123456");
    string resultText = Yaml::getValue(node, "name");
    EXPECT_EQ(resultText, "Leve4SiblingName");
}

TEST(YamlTest, getNodeListByKey) {
    vector<Node> nodes = Yaml::getNodeListByKey(testYamlFile, "aKey");
    bool isEqual = false;
    for (auto i: nodes) {
        string value = Yaml::getValue(i, "aKey");
        if (value == "1234" or value == "12345" or value == "123456") isEqual = true;
    }
    EXPECT_EQ(isEqual, true);
}

TEST(YamlTest, getNodeListByKeyAndValue) {
    vector<Node> nodes = Yaml::getNodeListByKey(testYamlFile, "key", "value");
    // TODO: check difference between usages above and below
    bool isEqual = true;
    for (auto i: nodes) {
        string value = Yaml::getValue(i, "key");
        if (value != "value") isEqual = false;
    }
    EXPECT_EQ(isEqual, true);
}

TEST(YamlTest, getNodeByPath) {
    // TODO when we want to get from path, we need node->second but for value we need all node
    Node node = Yaml::getNodeByPath(testYamlFile, "NestedItems.Level2.Level3.Leve4Sibling");
    string resultText = Yaml::getValue(node, "name");
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
    ASSERT_TRUE(count(values.begin(), values.end(), "Level4Item1"));
    ASSERT_TRUE(count(values.begin(), values.end(), "Level4Item2"));
    ASSERT_TRUE(count(values.begin(), values.end(), "Level4Item3"));
    
    EXPECT_EQ(values.size(), 3);
}

TEST(YamlTest, getValueListViaYamlFile) {
    vector<string> values = Yaml::getValueList(testYamlFile, "aKey");
    ASSERT_TRUE(count(values.begin(), values.end(), "1234"));
    ASSERT_TRUE(count(values.begin(), values.end(), "12345"));
    ASSERT_TRUE(count(values.begin(), values.end(), "123456"));
    EXPECT_EQ(values.size(), 3);
}