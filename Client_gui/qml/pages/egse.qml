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

    Label {
        id: labelStatus
        anchors.top: labelEgse.bottom
        anchors.left: parent.left
        anchors.topMargin: 60
        anchors.leftMargin: 30

        color: "#ffffff"
        text: "Status:"
        font.pixelSize: 20
        font.family: "Segoe UI"
    }

    CustomToggleButton {
        id: buttonRemote
        width: 140
        height: 90
        opacity: 1
        text: "Remote"
        
        anchors.top: labelStatus.bottom
        anchors.left: parent.left
        anchors.topMargin: 20
        anchors.leftMargin: 30
        colorPressed: "#55aaff"
        colorMouseOver: "#50555f"
        colorDefault: "#40405f"
    }

    CustomToggleButton {
        id: buttonLocal
        width: 140
        height: 90
        opacity: 1
        text: "Local"
        
        anchors.top: labelStatus.bottom
        anchors.left: buttonRemote.right
        anchors.topMargin: 20
        anchors.leftMargin: 15
        colorPressed: "#55aaff"
        colorMouseOver: "#50555f"
        colorDefault: "#40405f"

        isOn: false
    }

    Label {
        id: labelConfigurationSetup
        anchors.top: buttonLocal.bottom
        anchors.left: parent.left
        anchors.topMargin: 60
        anchors.leftMargin: 30

        color: "#ffffff"
        text: "Configuration Setup:"
        font.pixelSize: 20
        font.family: "Segoe UI"
    }

    CustomToggleButton {
        id: buttonStarkit
        width: 140
        height: 90
        opacity: 1
        text: "STARKIT"
        
        anchors.top: labelConfigurationSetup.bottom
        anchors.left: parent.left
        anchors.topMargin: 20
        anchors.leftMargin: 30
        colorPressed: "#55aaff"
        colorMouseOver: "#50555f"
        colorDefault: "#40405f"
    }

    CustomToggleButton {
        id: buttonMubV1
        width: 140
        height: 90
        opacity: 1
        text: "MUB_V1"
        
        anchors.top: labelConfigurationSetup.bottom
        anchors.left: buttonStarkit.right
        anchors.topMargin: 20
        anchors.leftMargin: 15
        colorPressed: "#55aaff"
        colorMouseOver: "#50555f"
        colorDefault: "#40405f"

        isOn: false
    }

    Rectangle {
        id: separator1
        width: 3
        color: "#5a5a85"

        anchors.top: parent.top
        anchors.left: buttonLocal.right
        anchors.bottom: parent.bottom
        anchors.topMargin: 100
        anchors.leftMargin: 30
        anchors.bottomMargin: 30
    }

    CustomToggleButton {
        id: buttonBypassA
        width: 140
        height: 90
        opacity: 1
        text: "Bypass A"

        anchors.top: parent.top
        anchors.left: separator1.right
        anchors.topMargin: 135
        anchors.leftMargin: 30
        colorPressed: "#55aaff"
        colorMouseOver: "#50555f"
        colorDefault: "#40405f"

        isOn: false
    }

    CustomToggleButton {
        id: buttonBypassB
        width: 140
        height: 90
        opacity: 1
        text: "Bypass B"

        anchors.top: parent.top
        anchors.left: buttonBypassA.right
        anchors.topMargin: 135
        anchors.leftMargin: 15
        colorPressed: "#55aaff"
        colorMouseOver: "#50555f"
        colorDefault: "#40405f"
    }

    CustomToggleButton {
        id: buttonA1
        width: 140
        height: 90
        opacity: 1
        text: "buttonA1"

        anchors.top: buttonBypassA.bottom
        anchors.left: separator1.right
        anchors.topMargin: 30
        anchors.leftMargin: 30
        colorPressed: "#55aaff"
        colorMouseOver: "#50555f"
        colorDefault: "#40405f"

        isOn: false
    }

    CustomToggleButton {
        id: buttonB1
        width: 140
        height: 90
        opacity: 1
        text: "buttonB1"

        anchors.top: buttonBypassA.bottom
        anchors.left: buttonA1.right
        anchors.topMargin: 30
        anchors.leftMargin: 15
        colorPressed: "#55aaff"
        colorMouseOver: "#50555f"
        colorDefault: "#40405f"
    }

    CustomToggleButton {
        id: buttonA2
        width: 140
        height: 90
        opacity: 1
        text: "buttonA2"

        anchors.top: buttonA1.bottom
        anchors.left: separator1.right
        anchors.topMargin: 30
        anchors.leftMargin: 30
        colorPressed: "#55aaff"
        colorMouseOver: "#50555f"
        colorDefault: "#40405f"

        isOn: false
    }

    CustomToggleButton {
        id: buttonB2
        width: 140
        height: 90
        opacity: 1
        text: "buttonB2"

        anchors.top: buttonA1.bottom
        anchors.left: buttonA2.right
        anchors.topMargin: 30
        anchors.leftMargin: 15
        colorPressed: "#55aaff"
        colorMouseOver: "#50555f"
        colorDefault: "#40405f"
    }

    Rectangle {
        id: separator2
        width: 3
        color: "#5a5a85"

        anchors.top: parent.top
        anchors.left: buttonBypassB.right
        anchors.bottom: parent.bottom
        anchors.topMargin: 100
        anchors.leftMargin: 30
        anchors.bottomMargin: 30
    }

    CustomToggleButton {
        id: buttonX1
        width: 140
        height: 90
        opacity: 1
        text: "buttonX1"

        anchors.top: parent.top
        anchors.left: separator2.right
        anchors.topMargin: 135
        anchors.leftMargin: 30
        colorPressed: "#55aaff"
        colorMouseOver: "#50555f"
        colorDefault: "#40405f"

        isOn: false
    }

    CustomToggleButton {
        id: buttonY1
        width: 140
        height: 90
        opacity: 1
        text: "buttonY1"

        anchors.top: parent.top
        anchors.left: buttonX1.right
        anchors.topMargin: 135
        anchors.leftMargin: 15
        colorPressed: "#55aaff"
        colorMouseOver: "#50555f"
        colorDefault: "#40405f"
    }

    CustomToggleButton {
        id: buttonX2
        width: 140
        height: 90
        opacity: 1
        text: "buttonX2"

        anchors.top: buttonX1.bottom
        anchors.left: separator2.right
        anchors.topMargin: 30
        anchors.leftMargin: 30
        colorPressed: "#55aaff"
        colorMouseOver: "#50555f"
        colorDefault: "#40405f"

        isOn: false
    }

    CustomToggleButton {
        id: buttonY2
        width: 140
        height: 90
        opacity: 1
        text: "buttonY2"

        anchors.top: buttonX1.bottom
        anchors.left: buttonX2.right
        anchors.topMargin: 30
        anchors.leftMargin: 15
        colorPressed: "#55aaff"
        colorMouseOver: "#50555f"
        colorDefault: "#40405f"
    }

    CustomToggleButton {
        id: buttonX3
        width: 140
        height: 90
        opacity: 1
        text: "buttonX3"

        anchors.top: buttonX2.bottom
        anchors.left: separator2.right
        anchors.topMargin: 30
        anchors.leftMargin: 30
        colorPressed: "#55aaff"
        colorMouseOver: "#50555f"
        colorDefault: "#40405f"

        isOn: false
    }

    CustomToggleButton {
        id: buttonY3
        width: 140
        height: 90
        opacity: 1
        text: "buttonY3"

        anchors.top: buttonX2.bottom
        anchors.left: buttonX3.right
        anchors.topMargin: 30
        anchors.leftMargin: 15
        colorPressed: "#55aaff"
        colorMouseOver: "#50555f"
        colorDefault: "#40405f"
    }

    Connections {
        target: backend
    }
}
