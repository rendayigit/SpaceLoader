#include <gtest/gtest.h>

#include <iostream>
#include <iterator>
#include <ostream>
#include <string>

#include "../../lib/YAML/yaml.h"
#include "../Test_common.h"
#include "yaml-cpp/yaml.h"

using std::string;
using std::vector;
using YAML::Node;

TEST(YamlTest, ByTestPath) {
    string path = "../../tests/YAML/Test.yaml";
    if (access(path.c_str(), F_OK) != 0) {
        GTEST_FAIL() << "cannot find " << path;
    }
}

TEST(YamlTest, ByMultipleItemNode) {
    Node resultNodes = Yaml::getNodeByKey("../../tests/YAML/Test.yaml", "Item");

    vector<string> resulttexts = Yaml::getTextList(resultNodes, "#text");
    ASSERT_EQ(resulttexts.at(0), "item1");
    ASSERT_EQ(resulttexts.at(1), "item2");
}

TEST(YamlTest, ByAttribute) {
    Node result =
        Yaml::getNodeByKey("../../tests/YAML/Test.yaml", "-attributeName2", "AttributeValue2");
    ASSERT_EQ(result["#text"].as<string>(), "Level2Item1");
}

TEST(YamlTest, BySingleItemNode) {
    Node resultNode = Yaml::getNodeByKey("../../tests/YAML/Test.yaml", "SingleItemNode");
    ASSERT_EQ(resultNode["#text"].as<string>(), "This is a single item");
}

TEST(YamlTest, ByMultipleAttribute) {
    Node resultNode =
        Yaml::getNodeByKey("../../tests/YAML/Test.yaml", "-attributeName1", "AttributeValue1");
    vector<string> resultText = Yaml::getTextList(resultNode, "#text");
    ASSERT_EQ(resultText.at(0), "Level4Item1");
    ASSERT_EQ(resultText.at(1), "Level4Item2");
    ASSERT_EQ(resultText.at(2), "Level4Item3");
    ASSERT_EQ(resultText.size(), 3);
}

TEST(YamlTest, ByPath) {
    Node resultNode = Yaml::getNodeByPath("../../tests/YAML/Test.yaml",
                                          "NestedItems.Level2.Level3.Leve4.Level4Item");
    vector<string> resultText = Yaml::getTextList(resultNode, "#text");
    ASSERT_EQ(resultText.size(), 3);
    ASSERT_EQ(resultText.at(0), "Level4Item1");
    ASSERT_EQ(resultText.at(1), "Level4Item2");
    ASSERT_EQ(resultText.at(2), "Level4Item3");
}

TEST(YamlTest, ByRealYamlFile_1) {
    Node resultNode = Yaml::getNodeByKey("../../Setup/Paths.yaml", "bin_Dir");
    ASSERT_EQ(resultNode.as<string>(), "bin/");
}

TEST(YamlTest, ByRealYamlFile_2) {
    Node resultNode = Yaml::getNodeByKey("../../Setup/Paths.yaml", "server_cmds_Yaml");
    ASSERT_EQ(resultNode.as<string>(), "Setup/ServerCmds.yaml");
}

TEST(YamlTest, ByKeyGetList) {
    vector<Node> resultNodes = Yaml::getNodeListByKey("../../tests/YAML/Test.yaml", "ip");

    EXPECT_EQ(resultNodes.at(0).as<int>(), 1234);
    EXPECT_EQ(resultNodes.size(), 3);
}

TEST(YamlTest, ByMultipleKeyGetList) {
    vector<Node> resultNodes =
        Yaml::getNodeListByKey("../../tests/YAML/Test.yaml", "#text", "Level3Item3");
        
    EXPECT_EQ(resultNodes.size(), 2);
}