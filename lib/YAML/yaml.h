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
     * @brief Get the Node By given file path and tag name
     * 
     * @param    yamlFilePath        The path where yaml file is located
     * @param    tagName             The tag name the desired node must have.
     * @return YAML::Node 
     */
    static YAML::Node getNodeByTag(const std::string &yamlFilePath, const std::string &tagName);

    /**
     * @brief Get the Node By given file path, tag name and tag value
     * 
     * @param    yamlFilePath        The path where yaml file is located
     * @param    tagName             The tag name the desired node must have.
     * @param    tagValue            The tag value the desired node must have.
     * @return YAML::Node 
     */
    static YAML::Node getNodeByTag(const std::string &yamlFilePath, const std::string &tagName,
                                   const std::string &tagValue);

    /**
     * @brief Get the Node List By given file path and tag name
     * 
     * @param    yamlFilePath        The path where yaml file is located
     * @param    tagName             The tag name the desired node must have.
     * @return std::vector<YAML::Node> 
     */
    static std::vector<YAML::Node> getNodeListByTag(const std::string &yamlFilePath,
                                                    const std::string &tagName);

    /**
     * @brief Get the Node List By given file path, tag name and tag value
     * 
     * @param    yamlFilePath        The path where yaml file is located
     * @param    tagName             The tag name the desired node must have.
     * @param    tagValue            The tag value the desired node must have.
     * @return std::vector<YAML::Node> 
     */
    static std::vector<YAML::Node> getNodeListByTag(const std::string &yamlFilePath,
                                                    const std::string &tagName,
                                                    const std::string &tagValue);

    /**
     * @brief Get the Node By file path and node path
     * 
     * @param    yamlFilePath        The path where yaml file is located
     * @param    path                The tag name the desired node must have.
     * @return YAML::Node 
     */
    static YAML::Node getNodeByPath(const std::string &yamlFilePath, const std::string &path);

    /**
     * @brief Get the Node List By given Node path
     * 
     * @param    yamlFilePath        The path where yaml file is located
     * @param    path                The node path which seperated by "." 
     * @return std::vector<YAML::Node> 
     */
    static std::vector<YAML::Node> getNodeListByPath(const std::string &yamlFilePath,
                                                     const std::string &path);

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

    /**
     * @brief Get the Text List of the given yaml node and tag name
     * 
     * @param    node                The yaml node to start searching for the node to retrieve the text from.
     * @param    tagName             The tag name the desired node must have.
     * @return std::vector<std::string> 
     */
    static std::vector<std::string> getTextList(const YAML::Node &node, const std::string &tagName);

    /**
     * @brief Get the Text List by given file path and tag name
     * 
     * @param    yamlFilePath        The path where yaml file is located
     * @param    tagName             The tag name the desired node must have.
     * @return std::vector<std::string> 
     */
    static std::vector<std::string> getTextList(const std::string &yamlFilePath,
                                                const std::string &tagName);

   private:
    Yaml() = default;
    ~Yaml() = default;

    /**
     * @brief Searching and returning nodes by path
     * 
     * @param    node                The yaml node to start searching for the node to retrieve the text from.
     * @param    pathOrder           The vector of node tags in order
     * @return std::vector<YAML::Node> 
     */
    static std::vector<YAML::Node> searchByNodePath(YAML::Node node,
                                                    std::vector<std::string> pathOrder);

    /**
     * @brief Searching and returning nodes by the given node, tagname and tagvalue
     * 
     * @param    node                The yaml node to start searching for the node to retrieve the text from.
     * @param    tagName             The tag name the desired node must have.
     * @param    tagValue            The tag value the desired node must have.
     * @return std::vector<YAML::Node> 
     */
    static std::vector<YAML::Node> searchNodeByTag(YAML::Node node, const std::string &tagName,
                                                   const std::string &tagValue);

    /**
     * @brief Searching and returning nodes by the given node and tagname
     * 
     * @param    node                The yaml node to start searching for the node to retrieve the text from.
     * @param    tagName             The tag name the desired node must have.
     * @return std::vector<YAML::Node> 
     */
    static std::vector<YAML::Node> searchNodeByTag(YAML::Node node, const std::string &tagName);

    /**
     * @brief Searching and returning texts by the given node and tagname
     * 
     * @param    node                The yaml node to start searching for the node to retrieve the text from.
     * @param    tagName             The tag name the desired node must have.
     * @return std::vector<std::string> 
     */
    static std::vector<std::string> searchText(const YAML::Node &node, const std::string &tagName);

    /**
     * @brief Split path string by given delimiter character
     * 
     * @param    path                The node path which seperated by "." 
     * @param    delimiter           The character which is using for seperate path
     * @return std::vector<std::string> 
     */
    static std::vector<std::string> splitPath(const std::string &path, char delimiter);
};

#endif  // YAML_H