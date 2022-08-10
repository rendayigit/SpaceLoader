import QtQuick 2.15
import QtQuick.Controls 2.15

CustomButton {
    property var isOn: true

    font.pixelSize: 24
    font.family: "Segoe UI"

    Rectangle {
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.leftMargin: 10
        anchors.rightMargin: 10
        anchors.bottomMargin: 10

        radius: 10
        height: 5

        color: (isOn) ? "#FF0000" : "#50555f"
    }
}
