import QtQuick 2.0
import "../components"
import QtQuick.Timeline 1.0
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtQuick.Window 2.15

Window {
    id: window

    // connect to homePage components
    property CustomButton hp_accessBtn : accessBtn
    property Label hp_mubAvailability : mubAvailability

    width: 600
    height: 400

    title: qsTr("Debugger")

    // Remove title bar
    flags: Qt.Window | Qt.FramelessWindowHint
    color: "#00000000"

    Component.onCompleted: {
        hp_accessBtn.enabled = false
        hp_accessBtn.colorDefault = "#222222"
        hp_mubAvailability.color = "#0000ff"
        hp_mubAvailability.text = "Accessed"
    }

    Component.onDestruction: {
        hp_accessBtn.enabled = true
        hp_accessBtn.colorDefault = "#33334c"
        hp_mubAvailability.color = "#00ff00"
        hp_mubAvailability.text = "Available"
    }

    Rectangle {
        anchors.fill: parent
        radius: 10
        color: "#27273a"

        DraggablePanel { target: window }

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
                    text: "Debugging with GDB\n > "
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

            TopBarButton {
                id: btnClose

                visible: true
                anchors.top: parent.top
                anchors.right: parent.right
                anchors.topMargin: 8
                anchors.rightMargin: 8

                btnColorDefault: "#dd0000"
                btnColorClicked: "#55aaff"
                btnColorMouseOver: "#ff007f"
                btnIconSource: "../../assets/svg_images/close_icon.svg"

                CustomToolTip {
                    text: "Close"
                }

                onPressed: window.destroy()
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
