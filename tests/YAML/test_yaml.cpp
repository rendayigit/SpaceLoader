#include "../../lib/YAML/yaml.h"
#include "../Test_common.h"
#include "yaml-cpp/yaml.h"

using std::string;
using std::vector;
using YAML::Node;

// TODO add tests for the new functions

TEST(YamlTest, ByTestPath) {
    string path = "../../tests/YAML/Test.yaml";
    if (access(path.c_str(), F_OK) != 0) {
        GTEST_FAIL() << "cannot find " << path;
    }
}

TEST(YamlTest, ByMultipleItemNode) {
    Node resultNodes = Yaml::getNodeByTag("../../tests/YAML/Test.yaml", "Item");

    vector<string> resulttexts = Yaml::getTextList(resultNodes, "#text");
    ASSERT_EQ(resulttexts.at(0), "item1");
    ASSERT_EQ(resulttexts.at(1), "item2");
}

TEST(YamlTest, ByAttribute) {
    Node result =
        Yaml::getNodeByTag("../../tests/YAML/Test.yaml", "-attributeName2", "AttributeValue2");
    string resultText = Yaml::getText(result, "#text");
    ASSERT_EQ(resultText, "Level2Item1");
}

TEST(YamlTest, BySingleItemNode) {
    Node resultNode = Yaml::getNodeByTag("../../tests/YAML/Test.yaml", "SingleItemNode");
    string resultText = Yaml::getText(resultNode, "#text");
    ASSERT_EQ(resultText, "This is a single item");
}

TEST(YamlTest, ByMultipleAttribute) {
    Node resultNode =
        Yaml::getNodeByTag("../../tests/YAML/Test.yaml", "-attributeName1", "AttributeValue1");
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
    ASSERT_EQ(resultText.at(0), "Level4Item1");
    ASSERT_EQ(resultText.at(1), "Level4Item2");
    ASSERT_EQ(resultText.at(2), "Level4Item3");
}

TEST(YamlTest, ByRealResult) {
    Node resultNode = Yaml::getNodeByTag("../../Setup/Paths.yaml", "bin_Dir");
    string resultText = Yaml::getText(resultNode, "bin_Dir");
    ASSERT_EQ(resultText, "bin/");
}

TEST(YamlTest, ByRealResult1) {
    Node resultNode = Yaml::getNodeByTag("../../Setup/Paths.yaml", "server_cmds_Yaml");
    string resultText = Yaml::getText(resultNode, "server_cmds_Yaml");
    ASSERT_EQ(resultText, "Setup/ServerCmds.yaml");
}