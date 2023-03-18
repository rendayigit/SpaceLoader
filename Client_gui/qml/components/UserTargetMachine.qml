import QtQuick 2.15
import QtQuick.Controls 2.15
import "../components"

Rectangle {
    property string targetMachineName: ""

    width: 200
    height: 50
    color: "#55AAFF"
    radius: 10

    Label {
        id: targetMachineNameLabel

        anchors.top: parent.top
        anchors.left: parent.left
        anchors.topMargin: 10
        anchors.leftMargin: 10

        color: "#000000"
        text: targetMachineName
        font.pixelSize: 20
        font.family: "Segoe UI"
    }

    CustomButton {
        width: 50

        anchors.right: parent.right
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.margins: 5

        font.pointSize: 10
        colorDefault: "#ff0000"
        colorMouseOver: "#ff0080"
        colorPressed: "#ff00ff"
        text: "Kick"
    }
}
