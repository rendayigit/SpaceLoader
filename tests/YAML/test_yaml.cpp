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
    vector<YAML::Node> resultNodes =
        Yaml::getNodeByTag<vector<YAML::Node>>("../../tests/YAML/Test.yaml", "Item");

    vector<string> resulttexts = Yaml::getText<vector<string>>(resultNodes.at(0), "#text");
    ASSERT_EQ(resulttexts.at(0), "item1");
    ASSERT_EQ(resulttexts.at(1), "item2");
}

TEST(yaml_test, ByAttribute) {
    vector<YAML::Node> result = Yaml::getNodeByTag<vector<YAML::Node>>(
        "../../tests/YAML/Test.yaml", "-attributeName2", "AttributeValue2");
    vector<string> resultText = Yaml::getText<vector<string>>(result.at(0), "#text");
    ASSERT_EQ(resultText.at(0), "Level2Item1");
}

TEST(yaml_test, BySingleItemNode) {
    config = YAML::LoadFile("../../tests/YAML/Test.yaml");
    vector<YAML::Node> resultNode =
        Yaml::getNodeByTag<vector<YAML::Node>>("../../tests/YAML/Test.yaml", "SingleItemNode");
    vector<string> resultText = Yaml::getText<vector<string>>(resultNode.at(0), "#text");
    ASSERT_EQ(resultText.at(0), "This is a single item");
}

TEST(yaml_test, ByMultipleAttribute) {
    config = YAML::LoadFile("../../tests/YAML/Test.yaml");
    vector<YAML::Node> resultNode = Yaml::getNodeByTag<vector<YAML::Node>>(
        "../../tests/YAML/Test.yaml", "-attributeName1", "AttributeValue1");
    vector<string> resultText = Yaml::getText<vector<string>>(resultNode.at(0), "#text");
    ASSERT_EQ(resultText.at(0), "Level4Item1");
    ASSERT_EQ(resultText.at(1), "Level4Item2");
    ASSERT_EQ(resultText.at(2), "Level4Item3");
    ASSERT_EQ(resultText.size(), 3);
}

TEST(yaml_test, ByPath) {
    vector<YAML::Node> resultNode = Yaml::getNodeByPath<vector<YAML::Node>>(
        "../../tests/YAML/Test.yaml", "NestedItems.Level2.Level3.Leve4.Level4Item");
    vector<string> resultText = Yaml::getText<vector<string>>(resultNode.at(0), "#text");
    ASSERT_EQ(resultText.at(0), "Level4Item1");
    ASSERT_EQ(resultText.at(1), "Level4Item2");
    ASSERT_EQ(resultText.at(2), "Level4Item3");
}

TEST(yaml_test, ByRealResult) {
    vector<YAML::Node> resultNode =
        Yaml::getNodeByTag<vector<YAML::Node>>("../../Setup/Paths.yaml", "bin_Dir");
    vector<string> resultText = Yaml::getText<vector<string>>(resultNode.at(0), "bin_Dir");
    ASSERT_EQ(resultText.at(0), "bin/");
}

TEST(yaml_test, ByRealResult1) {
    vector<YAML::Node> resultNode = Yaml::getNodeByTag<vector<YAML::Node>>(
        "../../Setup/Paths.yaml", "server_cmds_Yaml");
    vector<string> resultText = Yaml::getText<vector<string>>(resultNode.at(0), "server_cmds_Yaml");
    ASSERT_EQ(resultText.at(0), "Setup/ServerCmds.yaml");
}

TEST(yaml_test, ByRealResult2) {
    vector<YAML::Node> resultNode = Yaml::getNodeByTag<vector<YAML::Node>>(
        "../../Setup/Paths.yaml", "client_cmds_Yaml");
    vector<string> resultText = Yaml::getText<vector<string>>(resultNode.at(0), "client_cmds_Yaml");
    ASSERT_EQ(resultText.at(0), "Setup/ClientCmds.yaml");
}

TEST(yaml_test, ByRealResult3) {
    vector<YAML::Node> resultNode =
        Yaml::getNodeByTag<vector<YAML::Node>>("../../Setup/Paths.yaml", "config_Yaml");
    vector<string> resultText = Yaml::getText<vector<string>>(resultNode.at(0), "config_Yaml");
    ASSERT_EQ(resultText.at(0), "Setup/Config.yaml");
}


TEST(yaml_test, ByRealResult6){
    vector<YAML::Node> resultNode =
        Yaml::getNodeByTag<vector<YAML::Node>>("../../Setup/Paths.yaml", "config_Yaml");
    vector<string> resultText = Yaml::getText<vector<string>>(resultNode.at(0), "config_Yaml");
    ASSERT_EQ(resultText.at(0), "Setup/Config.yaml");
}