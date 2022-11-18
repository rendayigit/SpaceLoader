import QtQuick 2.15
import QtQuick.Controls 2.15

Item {
    id: itemId

    property string userIp: "This is a string"
    property string username: "This is a string"

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
                height: 200
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

        }

        Column {
            id: connectionColumn

            width: connectionColumn.width = itemId.width * 0.4
            height: connectionColumn.height = itemId.height
        }

    }
}
