import QtQuick 2.0
import "../components"
import QtQuick.Timeline 1.0
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

Rectangle {
    property string buffer: ""
    anchors.fill: parent
    radius: 10
    color: "#27273a"

    Label {
        id: labelEgse
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.topMargin: 30
        anchors.leftMargin: 30

        color: "#ffffff"
        text: "SSBV EGSE CONTROL INTERFACE"
        font.pixelSize: 30
        font.family: "Segoe UI"
    }

    CustomToggleButton {
        id: remote
        width: 140
        height: 90
        opacity: 1
        text: "Remote"
        anchors.top: labelEgse.bottom
        anchors.left: parent.left
        anchors.topMargin: 30
        anchors.leftMargin: 30
        colorPressed: "#55aaff"
        colorMouseOver: "#50555f"
        colorDefault: "#40405f"
    }

    CustomToggleButton {
        id: bypassA
        width: 140
        height: 90
        opacity: 1
        text: "Bypass A"
        anchors.top: remote.bottom
        anchors.left: parent.left
        anchors.topMargin: 20
        anchors.leftMargin: 30
        colorPressed: "#55aaff"
        colorMouseOver: "#50555f"
        colorDefault: "#40405f"
        isOn: false
    }

    CustomToggleButton {
        id: bypassB
        width: 140
        height: 90
        opacity: 1
        text: "Bypass B"
        anchors.top: remote.bottom
        anchors.left: bypassA.right
        anchors.topMargin: 20
        anchors.leftMargin: 15
        colorPressed: "#55aaff"
        colorMouseOver: "#50555f"
        colorDefault: "#40405f"
    }

    Connections {
        target: backend

    }
}
