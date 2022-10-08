import QtQuick 2.15
import QtQuick.Controls 2.15

CustomButton {
    property bool isOn: true

    font.pixelSize: 24
    font.family: "Segoe UI"

    textColor: (isOn) ? "#FF0000" : "#888888"

    Rectangle {
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.leftMargin: 10
        anchors.rightMargin: 10
        anchors.bottomMargin: 10

        radius: 10
        height: 3

        color: (isOn) ? "#FF0000" : "#50555f"
    }

    Rectangle {
        property int borderSize: 3

        color: "#00000000"
        radius: 15
        
        border.color: "#5a5a85"
        border.width: borderSize

        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.leftMargin: -borderSize
        anchors.rightMargin: -borderSize
        anchors.topMargin: -borderSize
        anchors.bottomMargin: -borderSize
    }
}
