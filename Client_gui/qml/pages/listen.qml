import QtQuick 2.0
import "../components"
import QtQuick.Timeline 1.0
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

Item {
    Component.onCompleted: backend.stopListen()

    Rectangle {
        anchors.fill: parent
        radius: 10
        color: "#27273a"

        CustomTextField {
            id: ip
            height: 40

            anchors.top: parent.top
            anchors.left: parent.left

            anchors.topMargin: 20
            anchors.leftMargin: 20

            text: "127.0.0.1:1235"

            font.pointSize: 12

            Component.onCompleted: {
                ip.text = backend.getConfigValue("Config.Ips.listenTo", "ip")
            }
        }

        CustomButton {
            id: listen
            anchors.top: parent.top
            anchors.left: ip.right

            anchors.topMargin: 20
            anchors.leftMargin: 20

            text: "Listen"
            width: 100
            height: 40

            colorDefault: "#4632a8"
            colorMouseOver: "#5643b5"
            colorPressed: "#7963e6"

            onClicked: {
                enabled = false
                stopListen.enabled = true
                textArea.text = "Listening to " + ip.text + "\n"
                // backend.listen(ip.text)
                backend.updateYamlFile("Config.Ips.listenTo", ip.text);
            }
        }

        CustomButton {
            id: stopListen
            anchors.top: parent.top
            anchors.left: listen.right

            anchors.topMargin: 20
            anchors.leftMargin: 20

            text: "Stop"
            width: 100
            height: 40

            colorDefault: "#4632a8"
            colorMouseOver: "#5643b5"
            colorPressed: "#7963e6"

            enabled: false

            onClicked: {
                listen.enabled = true
                enabled = false
                backend.stopListen()
            }
        }

        Rectangle {
            anchors.top: ip.bottom
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
                    font.family: "Segoe UI"
                    font.pointSize: 11
                    textFormat: TextEdit.PlainText
                }
            }
        }
    }

    Connections {
        target: backend

        function onGetListenerText(text) {
            textArea.text += text + "\n"
            textArea.cursorPosition += textArea.text.length
        }
    }
}

/*##^##
Designer {
    D{i:0;autoSize:true;height:480;width:640}
}
##^##*/
