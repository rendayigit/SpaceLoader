import QtQuick 2.15
import QtQuick.Controls 2.15

Item {
    id: itemId

    property string userIp: "Null"
    property string username: "Null"

    width: 1057
    height: 200

    Rectangle {
        id: rectangle

        width: rectangle.width = itemId.width
        height: 200
        color: "#27273a"
        radius: 20
        border.color: "#ffffff"
        layer.smooth: false

        Row {
            id: row

            width: row.width = itemId.width
            height: row.height = itemId.height
            spacing: 50

            Column {
                id: userColumn

                width: rectangle.width * 0.4
                height: itemId.height
                topPadding: userColumn.topPadding = userColumn.height * 0.2
                leftPadding: userColumn.leftPadding = userColumn.width * 0.1
                transformOrigin: Item.Center
                spacing: 35

                Text {
                    id: userIpText

                    width: userIpText.width = userColumn.width * 0.8
                    color: "#ffffff"
                    text: qsTr(userIp)
                    font.pixelSize: 24
                    horizontalAlignment: Text.AlignLeft
                    verticalAlignment: Text.AlignTop
                }

                Text {
                    id: usernameText

                    width: usernameText.width = userColumn.width * 0.8
                    color: "#ffffff"
                    text: qsTr(username)
                    font.pixelSize: 24
                    horizontalAlignment: Text.AlignLeft
                    verticalAlignment: Text.AlignVCenter
                }

            }

            ScrollView {
                id: scroll

                width: scroll.width = itemId.width * 0.4
                height: scroll.height = itemId.height
                // ScrollBar.vertical.policy: ScrollBar.AlwaysOn
                ScrollBar.horizontal.policy: ScrollBar.AlwaysOff
                bottomPadding: 20
                clip: true

                Column {
                    id: connectionColumn

                    width: parent.width
                    height: parent.height
                    topPadding: 25
                    bottomPadding: 25
                    leftPadding: 20
                    transformOrigin: Item.Center
                    spacing: 20
                    Component.onCompleted: () => {
                        var item = Qt.createComponent("../components/CustomConnectItem.qml").createObject(connectionColumn);
                        var item = Qt.createComponent("../components/CustomConnectItem.qml").createObject(connectionColumn);
                    }
                }

            }

        }

    }

}
