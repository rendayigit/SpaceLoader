import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtGraphicalEffects 1.15
import "../components"
import QtQuick.Timeline 1.0

Item {

    property bool showValue: true

    Flickable {
        id: flickable
        opacity: 0
        anchors.fill: parent
        contentHeight: gridLayout.height
        clip: true

        GridLayout {
            id: gridLayout
            columns: 1
            anchors.leftMargin: 0
            anchors.rightMargin: 0
            columnSpacing: 10
            rows: 3

            Rectangle{
                id: rectangle1
                color: "#27273a"
                height: 220
                width: flickable.width
                radius: 10

                Rectangle {
                    id: whiteCard_1
                    width: 340
                    color: "#545469"
                    radius: 10
                    anchors.left: parent.left
                    anchors.top: parent.top
                    anchors.bottom: parent.bottom
                    Image {
                        id: iconCart2
                        width: 175
                        height: 175
                        visible: true
                        anchors.left: parent.left
                        anchors.top: parent.top
                        source: "../../assets/images/gk2.PNG"
                        fillMode: Image.PreserveAspectFit
                        sourceSize.height: 175
                        sourceSize.width: 175
                        anchors.leftMargin: 15
                        anchors.topMargin: 15
                        antialiasing: false
                    }

                    Label {
                        id: labelTitleBar8
                        anchors.right: parent.right
                        anchors.top: parent.top
                        anchors.rightMargin: 15
                        anchors.topMargin: 15
                        color: "#ffffff"
                        text: qsTr("STARKIT")
                        font.bold: true
                        font.family: "Segoe UI"
                        font.pointSize: 11
                    }

                    CustomButton {
                        x: 227
                        y: 153
                        width: 108
                        height: 30
                        text: "ACCESS"
                        enabled: false
                        anchors.right: parent.right
                        anchors.bottom: parent.bottom
                        colorMouseOver: "#40405f"
                        // colorDefault: "#33334c"
                        colorDefault: "#222222"
                        anchors.bottomMargin: 17
                        anchors.rightMargin: 15
                        colorPressed: "#55aaff"
                    }

                    anchors.bottomMargin: 10
                    anchors.topMargin: 10
                    anchors.leftMargin: 10
                }

                Label {
                    y: 20
                    color: "#ffffff"
                    text: qsTr("AIRBUS DEFENCE & SPACE STARKIT")
                    anchors.left: whiteCard_1.right
                    font.bold: true
                    anchors.leftMargin: 37
                    font.family: "Segoe UI"
                    font.pointSize: 11
                }

                Label {
                    id: labelAvailability
                    y: 40
                    color: "#ffffff"
                    text: qsTr("Availability: ")
                    anchors.left: whiteCard_1.right
                    font.bold: false
                    anchors.leftMargin: 37
                    font.family: "Segoe UI"
                    font.pointSize: 11
                }

                Label {
                    y: 40
                    color: "#ff0000"
                    text: qsTr("Not Available")
                    anchors.left: labelAvailability.right
                    font.bold: true
                    anchors.leftMargin: 5
                    font.family: "Segoe UI"
                    font.pointSize: 11
                }

                Label {
                    id: labelUseBy
                    y: 60
                    color: "#ffffff"
                    text: qsTr("In use by: ")
                    anchors.left: whiteCard_1.right
                    font.bold: false
                    anchors.leftMargin: 37
                    font.family: "Segoe UI"
                    font.pointSize: 11
                }

                Label {
                    y: 60
                    color: "#ffffff"
                    text: qsTr("Renda Yigit")
                    anchors.left: labelUseBy.right
                    font.bold: false
                    anchors.leftMargin: 5
                    font.family: "Segoe UI"
                    font.pointSize: 11
                }

                CustomButton {
                    id: starkitOnOffButton
                    width: 108
                    height: 30
                    text: "Turn Off"
                    anchors.right: parent.right
                    anchors.top: parent.top
                    colorMouseOver: "#40405f"
                    colorDefault: "#33334c"
                    anchors.topMargin: 17
                    anchors.rightMargin: 15
                    colorPressed: "#55aaff"
                }

                CustomButton {
                    width: 108
                    height: 30
                    text: "Restart"
                    anchors.right: starkitOnOffButton.left
                    anchors.top: parent.top
                    colorMouseOver: "#40405f"
                    colorDefault: "#33334c"
                    anchors.topMargin: 17
                    anchors.rightMargin: 10
                    colorPressed: "#55aaff"
                }
            }

            Rectangle{
                id: rectangle2
                color: "#27273a"
                height: 220
                width: flickable.width
                radius: 10

                Rectangle {
                    id: whiteCard_2
                    width: 340
                    color: "#545469"
                    radius: 10
                    anchors.left: parent.left
                    anchors.top: parent.top
                    anchors.bottom: parent.bottom
                    Image {
                        width: 175
                        height: 175
                        visible: true
                        anchors.left: parent.left
                        anchors.top: parent.top
                        source: "../../assets/images/gk2.PNG"
                        fillMode: Image.PreserveAspectFit
                        sourceSize.height: 175
                        sourceSize.width: 175
                        anchors.leftMargin: 15
                        anchors.topMargin: 15
                        antialiasing: false
                    }

                    Label {
                        anchors.right: parent.right
                        anchors.top: parent.top
                        anchors.rightMargin: 15
                        anchors.topMargin: 15
                        color: "#ffffff"
                        text: qsTr("MUB")
                        font.bold: true
                        font.family: "Segoe UI"
                        font.pointSize: 11
                    }

                    CustomButton {
                        x: 227
                        y: 153
                        width: 108
                        height: 30
                        text: "ACCESS"
                        anchors.right: parent.right
                        anchors.bottom: parent.bottom
                        colorMouseOver: "#40405f"
                        colorDefault: "#33334c"
                        anchors.bottomMargin: 17
                        anchors.rightMargin: 15
                        colorPressed: "#55aaff"
                    }

                    anchors.bottomMargin: 10
                    anchors.topMargin: 10
                    anchors.leftMargin: 10
                }

                Label {
                    y: 20
                    color: "#ffffff"
                    text: qsTr("TURKISH AEROSPACE MUB")
                    anchors.left: whiteCard_2.right
                    font.bold: true
                    anchors.leftMargin: 37
                    font.family: "Segoe UI"
                    font.pointSize: 11
                }

                Label {
                    id: labelAvailability2
                    y: 40
                    color: "#ffffff"
                    text: qsTr("Availability: ")
                    anchors.left: whiteCard_2.right
                    font.bold: false
                    anchors.leftMargin: 37
                    font.family: "Segoe UI"
                    font.pointSize: 11
                }

                Label {
                    y: 40
                    color: "#00ff00"
                    text: qsTr("Available")
                    anchors.left: labelAvailability2.right
                    font.bold: true
                    anchors.leftMargin: 5
                    font.family: "Segoe UI"
                    font.pointSize: 11
                }

                CustomButton {
                    id: mubOnOffButton
                    width: 108
                    height: 30
                    text: "Turn Off"
                    anchors.right: parent.right
                    anchors.top: parent.top
                    colorMouseOver: "#40405f"
                    colorDefault: "#33334c"
                    anchors.topMargin: 17
                    anchors.rightMargin: 15
                    colorPressed: "#55aaff"
                }

                CustomButton {
                    width: 108
                    height: 30
                    text: "Restart"
                    anchors.right: mubOnOffButton.left
                    anchors.top: parent.top
                    colorMouseOver: "#40405f"
                    colorDefault: "#33334c"
                    anchors.topMargin: 17
                    anchors.rightMargin: 10
                    colorPressed: "#55aaff"
                }
            }
        }
        ScrollBar.vertical: ScrollBar {
            id: control
            size: 0.3
            position: 0.2
            orientation: Qt.Vertical
            visible: flickable.moving || flickable.moving

            contentItem: Rectangle {
                implicitWidth: 6
                implicitHeight: 100
                radius: width / 2
                color: control.pressed ? "#55aaff" : "#40405f"
            }
        }
    }

    Timeline {
        id: timeline
        animations: [
            TimelineAnimation {
                id: timelineAnimation
                running: true
                loops: 1
                duration: 1000
                to: 1000
                from: 0
            }
        ]
        endFrame: 1000
        enabled: true
        startFrame: 0

        KeyframeGroup {
            target: flickable
            property: "opacity"
            Keyframe {
                frame: 550
                value: 1
            }

            Keyframe {
                frame: 0
                value: 0
            }
        }
    }
}
