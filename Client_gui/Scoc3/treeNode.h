#ifndef TREENODE_H
#define TREENODE_H

#include <string>
#include <vector>

class TreeNode {
   public:
    std::string name;
    int degree;
    std::string value;
    TreeNode *parent;
    // WRITE BRIEF
    TreeNode(std::string nameConstruct, int degreeConstruct) {
        parent = nullptr;
        name = nameConstruct;
        degree = degreeConstruct;
    }
    // WRITE BRIEF
    TreeNode(TreeNode *parentConstruct, std::string nameConstruct, int degreeConstruct) {
        parent = parentConstruct;
        name = nameConstruct;
        degree = degreeConstruct;
    }
    // WRITE BRIEF
    TreeNode(std::string nameConstruct, int degreeConstruct, std::string valueConstruct) {
        parent = nullptr;
        name = nameConstruct;
        degree = degreeConstruct;
        value = valueConstruct;
    }
    // WRITE BRIEF
    TreeNode(TreeNode *parentConstruct, std::string nameConstruct, int degreeConstruct,
             std::string valueConstruct) {
        parent = parentConstruct;
        name = nameConstruct;
        degree = degreeConstruct;
        value = valueConstruct;
    }
    std::vector<TreeNode> children;
    void addChild(TreeNode child) { children.push_back(child); }
};

#endif  // TREENODE_H