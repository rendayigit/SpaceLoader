import "../components"
import QtQuick 2.0
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtQuick.Timeline 1.0

Item {
    id: root

    Component.onCompleted: {
        backend.getUserList();
    }

    Rectangle {
        anchors.fill: parent
        radius: 10
        color: "#27273a"

        Label {
            id: usersTitleLabel
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.topMargin: 30
            anchors.leftMargin: 30

            color: "#ffffff"
            text: "Connected Users"
            font.pixelSize: 30
            font.family: "Segoe UI"
        }

        Rectangle {
            id: userListRectangle

            anchors.top: usersTitleLabel.bottom
            anchors.bottom: parent.bottom
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.topMargin: 10
            anchors.bottomMargin: 10
            anchors.leftMargin: 50
            anchors.rightMargin: 50
            radius: 10
            color: "#00000000"

            Flickable {
                anchors.fill: parent
                
                contentHeight: parent.height

                flickableDirection: Flickable.VerticalFlick
                boundsBehavior: Flickable.DragAndOvershootBounds
                maximumFlickVelocity: 50

                clip: true
                
                ScrollView {
                    anchors.fill: parent
    //                anchors.Margin: 4
                    // clip: true

                    Column {
                        id: usersColumn
                        spacing: 20
                        anchors.fill: parent
                    }
                }
            }

        }
    }

    onWidthChanged: {
        for (var a = 0; a < usersColumn.children.length; a++) {
            usersColumn.children[a].width = root.width - 100
        }
    }

    Connections {
        target: backend

        function onGetUsers(text) {
            var userList = text.split("),");

            //TODO - remove these

            for (var i = 0; i < userList.length; i++) {
                var userIp = userList[i].split(": ")[1].split(" (")[1].replace(")","");
                var userName = userList[i].split(": ")[1].split(" ")[0];
                
                Qt.createComponent("../components/CustomUserListItem.qml")
                .createObject(usersColumn, {
                                  "userName": userName,
                                  "userIp": userIp,
                                  "width": root.width - 100
                              });
            }
        }
    }
}
