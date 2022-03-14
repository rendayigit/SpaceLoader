#ifndef YAML_H
#define YAML_H

#include <yaml-cpp/yaml.h>

class Yaml {
   public:
    Yaml(const Yaml &) = delete;
    Yaml &operator=(Yaml const &) = delete;
    Yaml(Yaml &&) = delete;
    Yaml &operator=(Yaml &&) = delete;

    static YAML::Node getNodeByTag(const std::string &yamlFilePath, const std::string &tagName);

    static YAML::Node getNodeByTag(const std::string &yamlFilePath, const std::string &tagName,
                                   const std::string &tagValue);

    static std::vector<YAML::Node> getNodeListByTag(const std::string &yamlFilePath,
                                                    const std::string &tagName);

    static std::vector<YAML::Node> getNodeListByTag(const std::string &yamlFilePath,
                                                    const std::string &tagName,
                                                    const std::string &tagValue);

    static YAML::Node getNodeByPath(const std::string &yamlFilePath, const std::string &path);

    static std::vector<YAML::Node> getNodeListByPath(const std::string &yamlFilePath,
                                                     const std::string &path);

    // TODO: add doxygen comments like this to the rest of the file. Note that typing /** then
    // hitting enter fills out most of the comments.
    /**
     * @brief Get the text of the given yaml node.
     *
     * @param node - The yaml node to retrieve the text from.
     * @return string - The text of the given yaml node.
     */
    // static std::string getText(const YAML::Node &node);

    static std::string getText(const YAML::Node &node, const std::string &tagName);

    /**
     * @brief Get the text of the given yaml node.
     *
     * @param node - The yaml node to start searching for the node to retrieve the text from.
     * @param tagName - The tag name the desired node must have.
     * @param tagValue - The tag value the desired node must have.
     * @return string - The text of the found yaml node.
     */

    static std::string getText(const std::string &yamlFilePath, const std::string &tagName);

    static std::vector<std::string> getTextList(const YAML::Node &node, const std::string &tagName);

    static std::vector<std::string> getTextList(const std::string &yamlFilePath,
                                                const std::string &tagName);

   private:
    Yaml() = default;
    ~Yaml() = default;

    static std::vector<YAML::Node> searchByNodePath(YAML::Node node,
                                                    std::vector<std::string> pathOrder);

    static std::vector<YAML::Node> searchNodeByTag(YAML::Node node, const std::string &tagName,
                                                   const std::string &tagValue);

    static std::vector<YAML::Node> searchNodeByTag(YAML::Node node, const std::string &tagName);

    static std::vector<std::string> searchText(const YAML::Node &node, const std::string &tagName);

    static std::vector<std::string> splitPath(const std::string &path, char delimiter);
};

#endif  // YAML_H