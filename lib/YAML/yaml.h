#ifndef YAML_H
#define YAML_H

#include <yaml-cpp/yaml.h>

class Yaml {
   public:
    Yaml(const Yaml &) = delete;
    Yaml &operator=(Yaml const &) = delete;
    Yaml(Yaml &&) = delete;
    Yaml &operator=(Yaml &&) = delete;

    /**
     * @brief Get the yaml node in the given yaml file with the given key.
     *
     * @param    yamlFilePath        The path where the yaml file is located.
     * @param    key                 The yaml node key the desired node must have.
     * @return   YAML::Node
     */
    static YAML::Node getNodeByKey(const std::string &yamlFilePath, const std::string &key);

    /**
     * @brief Get the yaml node in the given yaml file with the given key and value.
     *
     * @param    yamlFilePath        The path where the yaml file is located.
     * @param    key                 The yaml node key the desired node must have.
     * @param    value               The value to the key the desired node must have.
     * @return   YAML::Node
     */
    static YAML::Node getNodeByKey(const std::string &yamlFilePath, const std::string &key,
                                   const std::string &value);

    /**
     * @brief Get the yaml node list in the given yaml file with the given key.
     *
     * @param    yamlFilePath        The path where the yaml file is located.
     * @param    key                 The yaml node key the desired nodes must have.
     * @return   std::vector<YAML::Node>
     */
    static std::vector<YAML::Node> getNodeListByKey(const std::string &yamlFilePath,
                                                    const std::string &key);

    /**
     * @brief Get the yaml node list in the given yaml file with the given key and value.
     *
     * @param    yamlFilePath        The path where the yaml file is located.
     * @param    key                 The yaml node key the desired nodes must have.
     * @param    value               The value to the key the desired node must have.
     * @return   std::vector<YAML::Node>
     */
    static std::vector<YAML::Node> getNodeListByKey(const std::string &yamlFilePath,
                                                    const std::string &key,
                                                    const std::string &value);

    /**
     * @brief Get the yaml node in the given yaml file with the given node path.
     *
     * @param    yamlFilePath        The path where the yaml file is located.
     * @param    path                The path to the desired node. Nodes must be separated by '.'
     *                               (...grandParentNode.parentNode.desiredNode)
     * @return   YAML::Node
     */
    static YAML::Node getNodeByPath(const std::string &yamlFilePath, const std::string &path);

    // TODO: check if this is possible
    /**
     * @brief Get the yaml node list in the given yaml file with the given node path.
     *
     * @param    yamlFilePath        The path where the yaml file is located.
     * @param    path                The path to the desired nodes. Nodes must be separated by '.'
     *                               (...grandParentNode.parentNode.desiredNode)
     * @return   std::vector<YAML::Node>
     */
    static std::vector<YAML::Node> getNodeListByPath(const std::string &yamlFilePath,
                                                     const std::string &path);

    /**
     * @brief Get the text of the given yaml node with the given key.
     *
     * @param    node                The yaml node to retrieve the text from.
     * @param    key                 The yaml node key the desired node must have.
     * @return   std::string
     */
    static std::string getText(const YAML::Node &node, const std::string &key);

    /**
     * @brief Get the text of the yaml node in the given yaml file with the given key.
     *
     * @param    yamlFilePath        The path where the yaml file is located.
     * @param    key                 The yaml node key the desired node must have.
     * @return   std::string
     */
    static std::string getText(const std::string &yamlFilePath, const std::string &key);

    /**
     * @brief Get the text list of the given yaml node with the given key.
     *
     * @param    node                The yaml node to retrieve the texts from.
     * @param    key                 The yaml node key the desired node must have.
     * @return   std::vector<std::string>
     */
    static std::vector<std::string> getTextList(const YAML::Node &node, const std::string &key);

    /**
     * @brief Get the text list of the yaml node in the given yaml file with the given key.
     *
     * @param    yamlFilePath        The path where the yaml file is located.
     * @param    key                 The yaml node key the desired node must have.
     * @return   std::vector<std::string>
     */
    static std::vector<std::string> getTextList(const std::string &yamlFilePath,
                                                const std::string &key);

   private:
    Yaml() = default;
    ~Yaml() = default;

    // Private Functions
    static std::vector<YAML::Node> searchByNodePath(YAML::Node node,
                                                    std::vector<std::string> pathOrder);

    static std::vector<YAML::Node> searchNodeByKey(YAML::Node node, const std::string &key,
                                                   const std::string &value);

    static std::vector<YAML::Node> searchNodeByKey(YAML::Node node, const std::string &key);

    static std::vector<std::string> searchText(const YAML::Node &node, const std::string &key);

    static std::vector<std::string> splitPath(const std::string &path, char delimiter);
};

#endif  // YAML_H