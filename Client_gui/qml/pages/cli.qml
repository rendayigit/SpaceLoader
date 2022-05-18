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
                anchors.margins: 10

                TextArea {
                    id: textArea
                    anchors.centerIn: parent
                    anchors.fill: parent
                    color: "#ffffff"
                    text: "Server Console ("+backend.getServerIP()+ ":1234)\n > "
                    font.family: "Segoe UI"
                    font.pointSize: 11
                    textFormat: TextEdit.PlainText

                    Keys.onReturnPressed: {
                        backend.getTerminalData(text)

                        text = text + "\n > "
                        cursorPosition += text.length
                    }
                }
            }
        }
    }

    Connections {
        target: backend

        function onGetReceivedText(text) {
            textArea.text += text + "\n > "
            textArea.cursorPosition += textArea.text.length
        }
    }
}

/*##^##
Designer {
    D{i:0;autoSize:true;height:480;width:640}
}
##^##*/
