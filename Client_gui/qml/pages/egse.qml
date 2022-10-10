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
        width: 140
        
        font.pointSize: 12
        placeholderText: "Device IP"
        text: "localhost"
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
        width: 60
        
        font.pointSize: 12
        placeholderText: "TC Port"
        text: "5004"
    }

    CustomButton {
        id: buttonConnect

        anchors.left: textDeviceTcPort.right
        anchors.top: labelEgse.bottom
        anchors.leftMargin: 10
        anchors.topMargin: 10

        width: 60
        height: 30

        colorMouseOver: "#40405f"
        colorDefault: "#222222"
        colorPressed: "#55aaff"

        text: "Connect"

        onClicked: backend.egseConnect(textDeviceIp.text, textDeviceTcPort.text)
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

        onClicked: backend.transmitEgseTc(text)
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

        onClicked: backend.transmitEgseTc(text)
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

        onClicked: backend.transmitEgseTc(text)
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

        onClicked: backend.transmitEgseTc(text)
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
        id: buttonToggleTmOn
        width: 140
        height: 90
        opacity: 1
        text: "TM CHAIN ON"

        anchors.top: parent.top
        anchors.left: separator1.right
        anchors.topMargin: 135
        anchors.leftMargin: 30
        colorPressed: "#55aaff"
        colorMouseOver: "#5a5a85"
        colorDefault: "#40405f"

        isOn: false

        onClicked: backend.transmitEgseTc(text)
    }

    CustomToggleButton {
        id: buttonToggleTmOff
        width: 140
        height: 90
        opacity: 1
        text: "TM CHAIN OFF"

        anchors.top: parent.top
        anchors.left: buttonToggleTmOn.right
        anchors.topMargin: 135
        anchors.leftMargin: 15
        colorPressed: "#55aaff"
        colorMouseOver: "#5a5a85"
        colorDefault: "#40405f"

        isOn: false

        onClicked: backend.transmitEgseTc(text)
    }

    CustomToggleButton {
        id: buttonToggleTcOn
        width: 140
        height: 90
        opacity: 1
        text: "TC CHAIN ON"

        anchors.top: buttonToggleTmOn.bottom
        anchors.left: separator1.right
        anchors.topMargin: 30
        anchors.leftMargin: 30
        colorPressed: "#55aaff"
        colorMouseOver: "#5a5a85"
        colorDefault: "#40405f"

        isOn: false

        onClicked: backend.transmitEgseTc(text)
    }

    CustomToggleButton {
        id: buttonToggleTcOff
        width: 140
        height: 90
        opacity: 1
        text: "TC CHAIN OFF"

        anchors.top: buttonToggleTmOn.bottom
        anchors.left: buttonToggleTcOn.right
        anchors.topMargin: 30
        anchors.leftMargin: 15
        colorPressed: "#55aaff"
        colorMouseOver: "#5a5a85"
        colorDefault: "#40405f"

        isOn: false

        onClicked: backend.transmitEgseTc(text)
    }

    CustomToggleButton {
        id: buttonTmMode1
        width: 140
        height: 90
        opacity: 1
        text: "TM Mode 1"

        anchors.top: buttonToggleTcOn.bottom
        anchors.left: separator1.right
        anchors.topMargin: 30
        anchors.leftMargin: 30
        colorPressed: "#55aaff"
        colorMouseOver: "#5a5a85"
        colorDefault: "#40405f"

        isOn: false

        onClicked: backend.transmitEgseTc(text)
    }

    CustomToggleButton {
        id: buttonTmMode2
        width: 140
        height: 90
        opacity: 1
        text: "TM Mode 2"

        anchors.top: buttonToggleTcOn.bottom
        anchors.left: buttonTmMode1.right
        anchors.topMargin: 30
        anchors.leftMargin: 15
        colorPressed: "#55aaff"
        colorMouseOver: "#5a5a85"
        colorDefault: "#40405f"

        isOn: false

        onClicked: backend.transmitEgseTc(text)
    }

    Rectangle {
        id: separator2
        width: 3
        color: "#5a5a85"

        anchors.top: parent.top
        anchors.left: buttonToggleTmOff.right
        anchors.bottom: parent.bottom
        anchors.topMargin: 100
        anchors.leftMargin: 30
        anchors.bottomMargin: 30
    }

    CustomToggleButton {
        id: buttonTmSourceA
        width: 140
        height: 90
        opacity: 1
        text: "TM SOURCE A"

        anchors.top: parent.top
        anchors.left: separator2.right
        anchors.topMargin: 135
        anchors.leftMargin: 30
        colorPressed: "#55aaff"
        colorMouseOver: "#5a5a85"
        colorDefault: "#40405f"

        isOn: false

        onClicked: backend.transmitEgseTc(text)
    }

    CustomToggleButton {
        id: buttonTmSourceB
        width: 140
        height: 90
        opacity: 1
        text: "TM SOURCE B"

        anchors.top: parent.top
        anchors.left: buttonTmSourceA.right
        anchors.topMargin: 135
        anchors.leftMargin: 15
        colorPressed: "#55aaff"
        colorMouseOver: "#5a5a85"
        colorDefault: "#40405f"

        isOn: false

        onClicked: backend.transmitEgseTc(text)
    }

    CustomToggleButton {
        id: buttonTcDestA
        width: 140
        height: 90
        opacity: 1
        text: "TC DEST A"

        anchors.top: buttonTmSourceA.bottom
        anchors.left: separator2.right
        anchors.topMargin: 30
        anchors.leftMargin: 30
        colorPressed: "#55aaff"
        colorMouseOver: "#5a5a85"
        colorDefault: "#40405f"

        isOn: false

        onClicked: backend.transmitEgseTc(text)
    }

    CustomToggleButton {
        id: buttonTcDestB
        width: 140
        height: 90
        opacity: 1
        text: "TC DEST B"

        anchors.top: buttonTmSourceA.bottom
        anchors.left: buttonTcDestA.right
        anchors.topMargin: 30
        anchors.leftMargin: 15
        colorPressed: "#55aaff"
        colorMouseOver: "#5a5a85"
        colorDefault: "#40405f"

        isOn: false

        onClicked: backend.transmitEgseTc(text)
    }

    CustomToggleButton {
        id: buttonAlwaysRf
        width: 140
        height: 90
        opacity: 1
        text: "ALWAYS RF"

        anchors.top: buttonTcDestA.bottom
        anchors.left: separator2.right
        anchors.topMargin: 30
        anchors.leftMargin: 30
        colorPressed: "#55aaff"
        colorMouseOver: "#5a5a85"
        colorDefault: "#40405f"

        isOn: false

        onClicked: backend.transmitEgseTc(text)
    }

    CustomToggleButton {
        id: buttonY3
        width: 140
        height: 90
        opacity: 1
        text: "-"

        anchors.top: buttonTcDestA.bottom
        anchors.left: buttonAlwaysRf.right
        anchors.topMargin: 30
        anchors.leftMargin: 15
        colorPressed: "#55aaff"
        colorMouseOver: "#5a5a85"
        colorDefault: "#40405f"

        isOn: false

        onClicked: backend.transmitEgseTc(text)
    }

    QtObject {
        id: functions

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

            if(message.includes("REMOTE")) {
                functions.setActiveness(buttonRemote, isAck)
                functions.setActiveness(buttonLocal, !isAck)
            } else if(message.includes("LOCAL")) {
                functions.setActiveness(buttonRemote, !isAck)
                functions.setActiveness(buttonLocal, isAck)
            } else if(message.includes("TOGGLETM,OFF")) {
                functions.setActiveness(buttonToggleTmOff, isAck)
                functions.setActiveness(buttonToggleTmOn, !isAck)
            } else if(message.includes("TOGGLETM,ON")) {
                functions.setActiveness(buttonToggleTmOff, !isAck)
                functions.setActiveness(buttonToggleTmOn, isAck)
            } else if(message.includes("TOGGLETC,OFF")) {
                functions.setActiveness(buttonToggleTcOff, isAck)
                functions.setActiveness(buttonToggleTcOn, !isAck)
            } else if(message.includes("TOGGLETC,ON")) {
                functions.setActiveness(buttonToggleTcOff, !isAck)
                functions.setActiveness(buttonToggleTcOn, isAck)
            } else if(message.includes("IPUCMD")) {
                functions.setActiveness(buttonAlwaysRf, isAck)
            } else if(message.includes("SETTMMODE,1")) {
                functions.setActiveness(buttonTmMode1, isAck)
                functions.setActiveness(buttonTmMode2, !isAck)
            } else if(message.includes("SETTMMODE,2")) {
                functions.setActiveness(buttonTmMode1, !isAck)
                functions.setActiveness(buttonTmMode2, isAck)
            } else if(message.includes("IPUSETTCGDEST,2")) {
                functions.setActiveness(buttonTcDestB, isAck)
                functions.setActiveness(buttonTcDestA, !isAck)
            } else if(message.includes("IPUSETTCGDEST,4")) {
                functions.setActiveness(buttonTcDestA, isAck)
                functions.setActiveness(buttonTcDestB, !isAck)
            } else if(message.includes("IPUSETTMASOURCE,2")) {
                functions.setActiveness(buttonTmSourceB, isAck)
                functions.setActiveness(buttonTmSourceA, !isAck)
            } else if(message.includes("IPUSETTMASOURCE,4")) {
                functions.setActiveness(buttonTmSourceA, isAck)
                functions.setActiveness(buttonTmSourceB, !isAck)
            }

            if(buttonTmSourceA.isOn && buttonTcDestA.isOn) {
                buttonStarkit.isOn = true
            } else {
                buttonStarkit.isOn = false
            }

            if(buttonTmSourceB.isOn && buttonTcDestB.isOn) {
                buttonMubV1.isOn = true
            } else {
                buttonMubV1.isOn = false
            }
        }
    }
}
