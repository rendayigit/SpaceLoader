#include "xml.h"

#include <QtCore/qdebug.h>
#include <QtXml/qdom.h>

#include <string>

QDomElement Xml::getDocument(const QString &xmlFile) {
    QDomElement docElem;

    QDomDocument doc("Comm");
    QFile file(xmlFile);

    if (!file.open(QIODevice::ReadOnly)) {
        QString error = "Error with xml file: " + xmlFile;
        qFatal(error.toStdString().c_str());
    }

    if (!doc.setContent(&file)) {
        QString error = "Error with xml file: " + xmlFile;
        qFatal(error.toStdString().c_str());
    }

    file.close();

    docElem = doc.documentElement();

    return docElem;
}

QList<QString> Xml::getNodeTextsByNodeName(const QString &xmlFile,
                                           const QString &NodeName) {
    QList<QString> nodeTextList;

    QDomNodeList domNodeList = getDocument(xmlFile).elementsByTagName(NodeName);

    for (int i = 0; i < domNodeList.length(); ++i) {
        nodeTextList.append(domNodeList.item(i).toElement().text());
    }

    return nodeTextList;
}

QList<QString> Xml::getNodeTextsByNodePath(const QString &xmlFile,
                                           const QString &NodePath) {
    QList<QString> nodeTextList;
    QList<QString> nodePathList;

    nodePathList.append(NodePath.mid(0, NodePath.indexOf(".")));

    QString tempPath = NodePath;

    for (int i = 0; i < NodePath.count("."); ++i) {
        tempPath = tempPath.mid(tempPath.indexOf(".") + 1, tempPath.length());
        int startIndex = 0;
        int endIndex = tempPath.indexOf(".");
        nodePathList.append(tempPath.mid(startIndex, endIndex));
    }

    QDomElement docElem = getDocument(xmlFile);

    QDomNode node = docElem;
    for (auto &tagName : nodePathList) {
        node = node.namedItem(tagName);
    }

    while (not node.isNull()) {
        QDomElement e = node.toElement();
        if (not e.isNull()) {
            nodeTextList.append(e.text());
        }
        node = node.nextSibling();
    }

    return nodeTextList;
}

QList<QDomElement> Xml::getNodeTextsByNodeAttribute(
    const QString &xmlFile, const QString &AttributeName,
    const QString &AttributeValue) {
    QList<QDomNode> list = GetChildNodes(getDocument(xmlFile));
    QList<QDomElement> retList;

    for (auto &i : list) {
        if (not i.hasAttributes()) {
            continue;
        }
        QDomAttr attribute = i.toElement().attributeNode(AttributeName);
        if (not attribute.isNull()) {
            if (attribute.value() == AttributeValue) {
                retList.append(attribute.ownerElement());
            }
        }
    }

    return retList;
}

QList<QDomNode> Xml::GetChildNodes(const QDomNode &parentNode) {
    QList<QDomNode> resultSet;

    QDomNode node = parentNode.firstChild();

    while (not node.isNull()) {
        if (not node.toElement().text().isNull()) {
            resultSet.append(node);
        }

        if (node.hasChildNodes()) {
            QList<QDomNode> tempList = GetChildNodes(node);
            for (auto &i : tempList) {
                resultSet.append(i);
            }
        }

        node = node.nextSibling();
    }

    return resultSet;
}
