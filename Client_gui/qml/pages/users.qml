import "../components"
import QtQuick 2.0
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtQuick.Timeline 1.0

Item {
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

            ScrollView {
                anchors.left: parent.left
                anchors.bottom: parent.bottom
                anchors.top: parent.top
                anchors.leftMargin: 4
                anchors.bottomMargin: 4
                anchors.topMargin: 4
                clip: true

                Column {
                    id: usersColumn
                    spacing: 20
                }
            }
        }
    }

    Connections {
        target: backend

        function onGetUsers(text) {
            var userList = text.split("),");

            //TODO - remove these
            userList.push("User #x: test user (127.0.0.1)")
            userList.push("User #x: test user (127.0.0.1)")
            userList.push("User #x: test user (127.0.0.1)")
            userList.push("User #x: test user (127.0.0.1)")
            userList.push("User #x: test user (127.0.0.1)")
            userList.push("User #x: test user (127.0.0.1)")
            userList.push("User #x: test user (127.0.0.1)")
            userList.push("User #x: test user (127.0.0.1)")

            for (var i = 0; i < userList.length; i++) {
                var userIp = userList[i].split(": ")[1].split(" (")[1].replace(")","");
                var userName = userList[i].split(": ")[1].split(" ")[0];
                
                var itemUserItem = Qt.createComponent("../components/CustomUserListItem.qml")
                .createObject(usersColumn, {
                                  "userName": userName,
                                  "userIp": userIp,
                                  "width": parent.width - 100
                              });
            }
        }
    }
}
