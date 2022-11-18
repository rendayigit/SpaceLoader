import "../components"
import QtQuick 2.0
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtQuick.Timeline 1.0

Item {
    Rectangle {
        id: rectangle

        anchors.fill: parent
        radius: 10
        color: "#27273a"

        Rectangle {
            id: userListRectangle

            anchors.top: parent.top
            anchors.bottom: parent.bottom
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.topMargin: 10
            anchors.bottomMargin: 10
            anchors.leftMargin: 10
            anchors.rightMargin: 10
            radius: 10
            color: "#000000"

            ScrollView {
                id: scrollView

                anchors.left: parent.left
                anchors.bottom: parent.bottom
                anchors.top: parent.top
                anchors.leftMargin: 4
                anchors.bottomMargin: 4
                anchors.topMargin: 4
                clip: true

                Column {
                    id: logsColumn

                    Component.onCompleted: () => {
                        for (var i = 0; i < 2; i++) {
                            var item = Qt.createComponent("../components/CustomUserListItem.qml").createObject(logsColumn, {
                                "width": userListRectangle.width,
                                "userIp": "User Ip: 127.0.0.1",
                                "username": "Username: mrtkr"
                            });
                        }
                    }
                }

            }

        }

    }

    Connections {
        target: backend
    }

}
