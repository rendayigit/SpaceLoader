#include <gtest/internal/gtest-port.h>
#include "../../lib/XML/xml.h"
#include "../Test_common.h"

const QString xmlFile = QDir::currentPath() + "/../../tests/XML/Test.xml";


// NOLINTNEXTLINE
TEST(XmlTest, ByMultipleItemNode) {
    QList<QString> nodeList = Xml::getNodeTextsByNodeName(xmlFile, "Item");
    ASSERT_EQ(nodeList[0].toStdString(), "item1");
    ASSERT_EQ(nodeList[1].toStdString(), "item2");
}

// NOLINTNEXTLINE
TEST(XmlTest, BySingleItemNode) {
    QList<QString> nodeList =
        Xml::getNodeTextsByNodeName(xmlFile, "SingleItemNode");
    ASSERT_EQ(nodeList[0].toStdString(), "This is a single item");
}

// NOLINTNEXTLINE
TEST(XmlTest,ByNodePath) {
    QList<QString> nodeList = Xml::getNodeTextsByNodePath(
        xmlFile, "NestedItems.Level2.Level3.Leve4.Level4Item");
    ASSERT_EQ(nodeList[0].toStdString(), "Level4Item1");
    ASSERT_EQ(nodeList[1].toStdString(), "Level4Item2");
    ASSERT_EQ(nodeList[2].toStdString(), "Level4Item3");
}

// NOLINTNEXTLINE
TEST(XmlTest,ByAttributeSingleItem) {
    QList<QDomElement> nodeList = Xml::getNodeTextsByNodeAttribute(
        xmlFile, "attributeName2", "AttributeValue2");
    ASSERT_EQ(nodeList[0].text(), "Level2Item1");
    ASSERT_EQ(nodeList.size(), 1);
}

// NOLINTNEXTLINE
TEST(XmlTest,ByAttributeMultipleItems) {
    QList<QDomElement> nodeList = Xml::getNodeTextsByNodeAttribute(
        xmlFile, "attributeName1", "AttributeValue1");
    ASSERT_EQ(nodeList[0].childNodes().at(0).toElement().text(), "Level4Item1");
    ASSERT_EQ(nodeList[0].childNodes().at(1).toElement().text(), "Level4Item2");
    ASSERT_EQ(nodeList[0].childNodes().at(2).toElement().text(), "Level4Item3");
    ASSERT_EQ(nodeList.size(), 1);
    ASSERT_EQ(nodeList[0].childNodes().size(), 3);
    
}
