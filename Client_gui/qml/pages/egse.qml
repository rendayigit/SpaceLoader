import QtQuick 2.0
import QtQuick.Timeline 1.0
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import "../components"

Rectangle {
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

    CustomTextField {
        id: textDeviceIp
        
        anchors.left: parent.left
        anchors.top: labelEgse.bottom
        anchors.leftMargin: 30
        anchors.topMargin: 10
        
        height: 40
        width: 170
        
        font.pointSize: 12
        placeholderText: "Device IP"
        text: "192.168.1.250"
    }

    Label {
        id: colon
        anchors.left: textDeviceIp.right
        anchors.top: labelEgse.bottom
        anchors.leftMargin: 5
        anchors.topMargin: 10

        color: "#ffffff"
        text: ":"
        font.pixelSize: 20
        font.family: "Segoe UI"
    }

    CustomTextField {
        id: textDeviceTcPort
        
        anchors.left: colon.right
        anchors.top: labelEgse.bottom
        anchors.leftMargin: 5
        anchors.topMargin: 10
        
        height: 40
        width: 80
        
        font.pointSize: 12
        placeholderText: "TC Port"
        text: "5004"
    }

    Label {
        id: labelStatus
        anchors.top: textDeviceIp.bottom
        anchors.left: parent.left
        anchors.topMargin: 40
        anchors.leftMargin: 30

        color: "#ffffff"
        text: "Status:"
        font.pixelSize: 20
        font.family: "Segoe UI"
    }

    Label {
        id: labelError
        anchors.top: textDeviceIp.bottom
        anchors.left: labelStatus.right
        anchors.topMargin: 40
        anchors.leftMargin: 10

        color: "#ffffff"
        text: ""
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
        colorMouseOver: "#5a5a85"
        colorDefault: "#40405f"

        isOn: false

        onClicked: functions.transmitEgseTc("TODO")
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
        colorMouseOver: "#5a5a85"
        colorDefault: "#40405f"

        isOn: false

        onClicked: functions.transmitEgseTc("TODO")
    }

    Label {
        id: labelConfigurationSetup
        anchors.top: buttonLocal.bottom
        anchors.left: parent.left
        anchors.topMargin: 30
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
        colorMouseOver: "#5a5a85"
        colorDefault: "#40405f"

        isOn: false

        onClicked: functions.transmitEgseTc("TODO")
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
        colorMouseOver: "#5a5a85"
        colorDefault: "#40405f"

        isOn: false

        onClicked: functions.transmitEgseTc("TODO")
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
        colorMouseOver: "#5a5a85"
        colorDefault: "#40405f"

        isOn: false

        onClicked: functions.transmitEgseTc("TODO")
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
        colorMouseOver: "#5a5a85"
        colorDefault: "#40405f"

        isOn: false

        onClicked: functions.transmitEgseTc("TODO")
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
        colorMouseOver: "#5a5a85"
        colorDefault: "#40405f"

        isOn: false

        onClicked: functions.transmitEgseTc("TODO")
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
        colorMouseOver: "#5a5a85"
        colorDefault: "#40405f"

        isOn: false

        onClicked: functions.transmitEgseTc("TODO")
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
        colorMouseOver: "#5a5a85"
        colorDefault: "#40405f"

        isOn: false

        onClicked: functions.transmitEgseTc("TODO")
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
        colorMouseOver: "#5a5a85"
        colorDefault: "#40405f"

        isOn: false

        onClicked: functions.transmitEgseTc("TODO")
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
        colorMouseOver: "#5a5a85"
        colorDefault: "#40405f"

        isOn: false

        onClicked: functions.transmitEgseTc("TODO")
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
        colorMouseOver: "#5a5a85"
        colorDefault: "#40405f"

        isOn: false

        onClicked: functions.transmitEgseTc("TODO")
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
        colorMouseOver: "#5a5a85"
        colorDefault: "#40405f"

        isOn: false

        onClicked: functions.transmitEgseTc("TODO")
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
        colorMouseOver: "#5a5a85"
        colorDefault: "#40405f"

        isOn: false

        onClicked: functions.transmitEgseTc("TODO")
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
        colorMouseOver: "#5a5a85"
        colorDefault: "#40405f"

        isOn: false

        onClicked: functions.transmitEgseTc("TODO")
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
        colorMouseOver: "#5a5a85"
        colorDefault: "#40405f"

        isOn: false

        onClicked: functions.transmitEgseTc("TODO")
    }

    QtObject {
        id: functions
        function transmitEgseTc(tc) {
            backend.transmitEgseTc(tc, textDeviceIp.text, textDeviceTcPort.text)
        }

        function setActiveness(id, isActive) {
            id.isOn = isActive
        }
    }

    Connections {
        target: backend

        function onEgseError(isError, text) {
            if (isError) {
                labelError.color = "#ff0000"
            } else {
                labelError.color = "#00ff00"
            }

            labelError.text = text
        }

        function onEgseReply(message) {
            var isAck = false

            if(message.includes("ACK")) {
                isAck = true
            }

            if(message.includes("remote")) {
                functions.setActiveness(buttonRemote, isAck)
                functions.setActiveness(buttonLocal, !isAck)
            } else if("message substring2") {
                functions.setActiveness(buttonA1, isAck)
            }
        }
    }
}
