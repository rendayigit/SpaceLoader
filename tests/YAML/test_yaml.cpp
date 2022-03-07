#include <unistd.h>

#include <iostream>
#include "../../lib/YAML/yaml.h"
#include "../Test_common.h"
#include "yaml-cpp/yaml.h"

using namespace std;

YAML::Node config = YAML::LoadFile("../../tests/YAML/Test.yaml");

TEST(yaml_test, ByTestPath) {
    string path = "../../tests/YAML/Test.yaml";
    if (access(path.c_str(), F_OK) == 0) {
        config = YAML::LoadFile(path);
    } else {
        cout << "cannot find " << path;
        FAIL();
    }
}

TEST(yaml_test, ByMultipleItemNode) {
    YAML::Node resultNodes =
        Yaml::getSingleNodeByTag<YAML::Node>("../../tests/YAML/Test.yaml", "Item");

    vector<string> resulttexts = Yaml::getMultipleText<vector<string>>(resultNodes, "#text");
    ASSERT_EQ(resulttexts.at(0), "item1");
    ASSERT_EQ(resulttexts.at(1), "item2");
}

TEST(yaml_test, ByAttribute) {
    YAML::Node result = Yaml::getSingleNodeByTag<YAML::Node>(
        "../../tests/YAML/Test.yaml", "-attributeName2", "AttributeValue2");
    string resultText = Yaml::getSingleText<string>(result, "#text");
    ASSERT_EQ(resultText, "Level2Item1");
}

TEST(yaml_test, BySingleItemNode) {
    config = YAML::LoadFile("../../tests/YAML/Test.yaml");
    YAML::Node resultNode =
        Yaml::getSingleNodeByTag<YAML::Node>("../../tests/YAML/Test.yaml", "SingleItemNode");
    string resultText = Yaml::getSingleText<string>(resultNode, "#text");
    ASSERT_EQ(resultText, "This is a single item");
}

TEST(yaml_test, ByMultipleAttribute) {
    config = YAML::LoadFile("../../tests/YAML/Test.yaml");
    YAML::Node resultNode = Yaml::getSingleNodeByTag<YAML::Node>(
        "../../tests/YAML/Test.yaml", "-attributeName1", "AttributeValue1");
    vector<string> resultText = Yaml::getMultipleText<vector<string>>(resultNode, "#text");
    ASSERT_EQ(resultText.at(0), "Level4Item1");
    ASSERT_EQ(resultText.at(1), "Level4Item2");
    ASSERT_EQ(resultText.at(2), "Level4Item3");
    ASSERT_EQ(resultText.size(), 3);
}

TEST(yaml_test, ByPath) {
    YAML::Node resultNode = Yaml::getSingleNodeByPath<YAML::Node>(
        "../../tests/YAML/Test.yaml", "NestedItems.Level2.Level3.Leve4.Level4Item");
    vector<string> resultText = Yaml::getMultipleText<vector<string>>(resultNode, "#text");
    ASSERT_EQ(resultText.at(0), "Level4Item1");
    ASSERT_EQ(resultText.at(1), "Level4Item2");
    ASSERT_EQ(resultText.at(2), "Level4Item3");
}

TEST(yaml_test, ByRealResult) {
    YAML::Node resultNode =
        Yaml::getSingleNodeByTag<YAML::Node>("../../Setup/Paths.yaml", "bin_Dir");
    string resultText = Yaml::getSingleText<string>(resultNode, "bin_Dir");
    ASSERT_EQ(resultText, "bin/");
}

TEST(yaml_test, ByRealResult1) {
    YAML::Node resultNode = Yaml::getSingleNodeByTag<YAML::Node>(
        "../../Setup/Paths.yaml", "server_cmds_Yaml");
    string resultText = Yaml::getSingleText<string>(resultNode, "server_cmds_Yaml");
    ASSERT_EQ(resultText, "Setup/ServerCmds.yaml");
}