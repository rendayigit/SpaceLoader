#pragma once

#include <QtXml/QDomDocument>
#include <QtXml/QtXml>

class Xml {
   public:
    static Xml &getInstance() {
        static Xml instance;
        return instance;
    }

    Xml(Xml const &) = delete;
    Xml(Xml const &&) = delete;
    Xml &operator=(Xml const &&) = delete;
    void operator=(Xml const &) = delete;

    /**
     * @brief Creates an xml element from the given xml path
     *
     * @param xmlFile Xml file path
     * @return Xml file element
     */
    static QDomElement getDocument(const QString &xmlFile);

    /**
     * @brief Returns a list of texts found in the given node
     *
     * @param xmlFile Xml file path
     * @param NodeName Node tag name
     * @return List of texts found in the specified node(s)
     */
    static QList<QString> getNodeTextsByNodeName(const QString &xmlFile,
                                                 const QString &NodeName);

    /**
     * @brief Returns a list of texts found in the given node
     *
     * @param xmlFile Xml file path
     * @param NodePath Path should be given as "Parent.Child.Child.TargetNode".
     * Do not specify the root node of the xml file
     * @return List of texts found in the specified node(s)
     */
    static QList<QString> getNodeTextsByNodePath(const QString &xmlFile,
                                                 const QString &NodePath);

    /**
     * @brief Returns a list of texts found in the given node
     *
     * @param xmlFile Xml file path
     * @param AttributeName Attribute Name
     * @param AttributeValue Attribute Value
     * @return List of texts found in the specified node(s)
     */
    static QList<QDomElement> getNodeTextsByNodeAttribute(
        const QString &xmlFile, const QString &AttributeName,
        const QString &AttributeValue);

   private:
    Xml() = default;

    ~Xml() = default;

    static QList<QDomNode> GetChildNodes(const QDomNode &parentNode);
};
