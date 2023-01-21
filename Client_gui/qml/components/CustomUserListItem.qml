import QtQuick 2.15
import QtQuick.Controls 2.15

Item {
    property string userName: ""
    property string userIp: ""
    property var connectedTargetMachines: ["Starkit", "MUB", "TEST1", "TEST2"] // TODO - get this data from server

    height: 100

    Component.onCompleted:{
        for (var i = 0; i < connectedTargetMachines.length; i++) {
            var targetMachine = Qt.createComponent("../components/UserTargetMachine.qml")
            .createObject(targetMachineRow, {
                              "targetMachineName": connectedTargetMachines[i]
                          });
        }
    }

    Rectangle {
        id: rectangle

        anchors.fill: parent

        color: "#49496b"
        radius: 20

        Label {
            id: userNameTitleLabel
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.topMargin: 15
            anchors.leftMargin: 20

            color: "#ffffff"
            text: "User: "
            font.pixelSize: 20
            font.family: "Segoe UI"
        }

        Label {
            id: userNameLabel
            anchors.top: parent.top
            anchors.left: userNameTitleLabel.right
            anchors.topMargin: 15
            anchors.leftMargin: 5

            color: "#00ff00"
            text: userName
            font.pixelSize: 20
            font.family: "Segoe UI"
        }

        Label {
            id: userIpTitleLabel
            anchors.top: userNameTitleLabel.bottom
            anchors.left: parent.left
            anchors.topMargin: 10
            anchors.leftMargin: 20

            color: "#ffffff"
            text: "IP: "
            font.pixelSize: 20
            font.family: "Segoe UI"
        }

        Label {
            id: userIpLabel
            anchors.top: userNameTitleLabel.bottom
            anchors.left: userIpTitleLabel.right
            anchors.topMargin: 10
            anchors.leftMargin: 5

            color: "#00ff00"
            text: userIp
            font.pixelSize: 20
            font.family: "Segoe UI"
        }

        Label {
            id: targetMachinesAcquiredLabel
            anchors.verticalCenter: parent.verticalCenter
            anchors.left: userIpLabel.right
            anchors.margins: 50

            color: "#ffffff"
            text: "User has acquired\nthe following targets"
            font.pixelSize: 20
            font.family: "Segoe UI"
        }

        ScrollView {
            id: scroll

            anchors.left: targetMachinesAcquiredLabel.right
            anchors.right: parent.right
            anchors.top: parent.top
            anchors.bottom: parent.bottom

            anchors.leftMargin: 20

            clip: true

            Row {
                id: targetMachineRow

                anchors.fill: parent

                topPadding: 25
                bottomPadding: 25
                leftPadding: 20
                transformOrigin: Item.Center
                spacing: 20
            }
        }
    }
}
