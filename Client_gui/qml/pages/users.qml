import QtQuick 2.0
import "../components"
import QtQuick.Timeline 1.0
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

Item {
    Rectangle {
        anchors.fill: parent
        radius: 10
        color: "#27273a"

        Component.onCompleted: {
            backend.getUserList()
        }

        Rectangle {
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
                anchors.top: parent.top
                anchors.bottom: parent.bottom
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.margins: 50

                TextArea {
                    id: textArea
                    anchors.centerIn: parent
                    anchors.fill: parent
                    color: "#ffffff"
                    font.family: "Segoe UI"
                    font.pointSize: 20
                    textFormat: TextEdit.PlainText
                }
            }
        }
    }

    Connections {
        target: backend

        function onGetUsers(text) {
            textArea.text = text
        }
    }
}

/*##^##
Designer {
    D{i:0;autoSize:true;height:480;width:640}
}
##^##*/
