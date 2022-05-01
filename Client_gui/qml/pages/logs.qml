import QtQuick 2.15
import "../components"
import QtQuick.Timeline 1.0
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

Item {
    id: item1

    Component.onCompleted: {
        backend.listLogs()
    }

    Rectangle {
        id: rectangle
        anchors.fill: parent
        radius: 10
        color: "#27273a"

        Rectangle {
            id: leftMenu
            width: 240
            color: "#00000000"
            border.color: "#00000000"
            border.width: 0
            anchors.left: parent.left
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 10
            anchors.leftMargin: 5
            anchors.topMargin: 10

            ScrollView {
                anchors.left: parent.left
                anchors.bottom: parent.bottom
                anchors.top: parent.top
                anchors.leftMargin: 4
                anchors.bottomMargin: 4
                anchors.topMargin: 4
                clip: true

                Column {
                    id: logsColumn
                }
            }
        }

        Rectangle {
            id: selectFile
            height: 150
            anchors.left: leftMenu.right
            anchors.right: parent.right
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            anchors.leftMargin: 150
            anchors.rightMargin: 150
            anchors.topMargin: 150
            anchors.bottomMargin: 150
            radius: 10
            color: "#49496b"

            Label {
                color: "#ffffff"
                text: qsTr("Select a log file from the left menu to display ..")
                font.family: "Segoe UI"
                font.pointSize: 11
                anchors.centerIn: parent
            }
        }

        Rectangle {
            id: logText
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            anchors.left: leftMenu.right
            anchors.right: parent.right
            anchors.topMargin: 10
            anchors.bottomMargin: 10
            anchors.leftMargin: 10
            anchors.rightMargin: 10
            visible: false

            radius: 10
            color: "#000000"

            ScrollView {
                id: view
                anchors.fill: parent

                TextArea {
                    id: logDisplay
                    anchors.fill: parent
                    color: "#ffffff"
                    font.family: "Segoe UI"
                    font.pointSize: 11
                    anchors.centerIn: parent
                }
            }
        }
    }

    Connections {
        target: backend

        function onClearLogs() {
            for(var i = 0 ; i < logsColumn.children.length; i++) {
                logsColumn.children[i].destroy()
            }

            logText.visible = false
            selectFile.visible = true
            logDisplay.text = "clear"
        }

        function onGetLogList(text) {
            Qt.createComponent("../components/LogButton.qml")
            .createObject(logsColumn, {
                              "text": text,
                              "btnIconSource": "../../assets/images/logs.png",
                              "Layout.alignment": Qt.AlignHCenter | Qt.AlignVCenter
                          });
        }

        function onGetLogText(text) {
            selectFile.visible = false
            logText.visible = true
            logDisplay.text = text
        }
    }
}
