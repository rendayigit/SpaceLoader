import QtQuick 2.15
import QtQuick.Window 2.15
import QtGraphicalEffects 1.15
import QtQuick.Timeline 1.0
import QtQuick.Controls 2.15
import QtMultimedia 5.15

import "components"

Window {
    width: 550
    height: 500
    visible: true
    id: splashScreen
    color: "#00000000"
    title: qsTr("Login SpaceLoader")
    signal timeout

    // Remove title bar
    flags: Qt.Window | Qt.FramelessWindowHint
    modality: Qt.ApplicationModal

    // Custom Properties
    property int timeoutInterval: 1000

    Component.onCompleted: {
        labelLocalIp.text += backend.getLocalIp()
    }

    Timer {
        id: timer
        interval: timeoutInterval;
        running: false;
        repeat: false
        onTriggered: {
            splashScreen.close()
            var component = Qt.createComponent("main.qml");
            var win = component.createObject(splashScreen)
            win.show()
        }
    }

    // Functions
    QtObject {
        id: internal

        // Verify Login
        function checkLogin() {
            // TODO - This is the password
            if (loginTextField.text == "1") {
                loginTextField.borderColor = "#00ff7f"
                labelPassword.visible = false
                loginAnimationFrameMarginTop.running = true
                timer.running = true
                backend.setServerIp(textServerIp.text)
            } else {
                loginTextField.borderColor = "#55aaff"
                labelPassword.visible = true
            }
        }

        // Reset Text Error
        function resetTextLogin() {
            labelPassword.visible = false
            loginTextField.borderColor = "#55aaff"
        }
    }

    Rectangle {
        id: bg
        x: 25
        y: 131
        width: 500
        height: 318
        opacity: 0
        color: "#101010"
        radius: 20
        border.color: "#40405f"
        border.width: 3
        anchors.verticalCenter: parent.verticalCenter
        clip: true
        anchors.verticalCenterOffset: -100
        anchors.horizontalCenter: parent.horizontalCenter
        z: 3

        DragHandler { onActiveChanged: if(active) { splashScreen.startSystemMove() } }

        CircularProgressBar {
            id: circularProgressBar;
            width: 200;
            height: 200;
            opacity: 1
            anchors.verticalCenter: parent.verticalCenter ;
            enableDropShadow: false
            value: 100;
            textColor: "#ffffff"
            progressColor: "#55aaff"
            progressWidth: 4;
            strokeBgWidth: 2;
            bgStrokeColor: "#40405f"
            anchors.horizontalCenter: parent.horizontalCenter
        }

        Video {
            id: logoImage
            x: 50
            y: 50
            width: 300
            height: 300
            anchors.verticalCenter: parent.verticalCenter
            source: "../assets/videos/earth.avi"
            anchors.horizontalCenter: parent.horizontalCenter
            fillMode: VideoOutput.PreserveAspectFit
            flushMode: VideoOutput.LastFrame
            autoPlay: true
            loops: MediaPlayer.Infinite
            playbackRate: 0.4
        }

        TopBarButton {
            id: btnClose;
            opacity: 0
            visible: true
            anchors.right: parent.right;
            anchors.top: parent.top;
            enabled: true
            btnColorClicked: "#55aaff"
            btnColorMouseOver: "#ff007f"
            btnIconSource: "../assets/svg_images/close_icon.svg";
            anchors.topMargin: 8
            anchors.rightMargin: 8
            onClicked: splashScreen.close()
            
            CustomToolTip {
                text: "Close"
            }
        }

        Image {
            id: labelName
            x: 350
            y: 273
            height: 70
            opacity: 0.5
            anchors.right: parent.right
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 20
            anchors.rightMargin: 30
            source: "../assets/images/TAI_logo_large.png"
            fillMode: Image.PreserveAspectFit
        }

        // CustomButton {
        //     id: btnChangeUser
        //     x: 350
        //     y: 270
        //     width: 140
        //     height: 30
        //     opacity: 1
        //     text: "CHANGE USER"
        //     anchors.right: parent.right
        //     anchors.bottom: parent.bottom
        //     anchors.bottomMargin: 20
        //     anchors.rightMargin: 30
        //     colorPressed: "#55aaff"
        //     colorMouseOver: "#40405f"
        //     colorDefault: "#40405f"
        //     CustomToolTip{
        //         text: "Change user"
        //     }
        // }

        Label {
            id: labelUnlockInfo1
            x: 30
            y: 23
            opacity: 0
            color: "#55aaff"
            text: "Turkish Aerospace SpaceLoader"
            textFormat: Text.RichText
            font.pointSize: 8
            font.bold: false
            font.family: "Segoe UI"
            font.weight: Font.Normal
        }

        // Image {
        //     id: image1
        //     x: 50
        //     y: 104
        //     width: 50
        //     height: 50
        //     opacity: 0
        //     anchors.verticalCenter: parent.verticalCenter
        //     source: "../assets/images/logo_white_50x50.png"
        //     sourceSize.height: 50
        //     sourceSize.width: 50
        //     fillMode: Image.PreserveAspectFit
        // }
    }

    Rectangle {
        id: loginFrame
        x: 45
        width: 460
        height: 180
        visible: false
        color: "#1d1d2b"
        radius: 15
        border.color: "#33334c"
        border.width: 3
        anchors.top: bg.bottom
        anchors.topMargin: -280
        anchors.horizontalCenter: parent.horizontalCenter
        z: 2

        PropertyAnimation {
            id: loginAnimationFrameMarginTop
            target: loginFrame
            property: "anchors.topMargin"
            to: -280
            duration: 950
            easing.type: Easing.InOutQuint
        }

        Label {
            id: labelServerIp
            x: 30
            y: 50
            color: "#ffffff"
            text: qsTr("Server IP:")
            font.weight: Font.DemiBold
            font.bold: true
            font.pointSize: 14
            font.family: "Segoe UI"
        }

        CustomTextField {
            id: textServerIp
            height: 30
            width: 120

            x: 120
            y: 47

            placeholderText: "server ip"
            text: "127.0.0.1"

            font.pointSize: 10
        }

        Label {
            id: labelLocalIp
            x: 30
            y: 150
            color: "#55aaff"
            text: "Local IP: "
            font.pointSize: 10
            font.bold: false
            font.family: "Segoe UI"
            font.weight: Font.Normal
        }

        Image {
            id: imageFingerPrint
            x: 185
            y: 123
            width: 100
            height: 100
            opacity: 0
            anchors.verticalCenter: parent.verticalCenter
            anchors.right: parent.right
            source: "../assets/images/fingerprint.png"
            anchors.rightMargin: 30
            anchors.verticalCenterOffset: 10
            fillMode: Image.PreserveAspectFit

            PropertyAnimation {
                id: imageAnimationRightMargin
                target: imageFingerPrint
                property: "anchors.rightMargin"
                to: if(imageFingerPrint.anchors.rightMargin == 30) return 130; else return 30
                duration: 5000
                easing.type: Easing.InOutQuint
            }

            PropertyAnimation {
                id: imageAnimationOpacity
                target: imageFingerPrint
                property: "opacity"
                to: if(imageFingerPrint.opacity == 0) return 0.35; else return 0
                duration: 5000
                easing.type: Easing.InOutQuint
            }
        }

        CustomTextField {
            id: loginTextField
            x: 261
            y: 80
            width: 160
            height: 40
            anchors.right: parent.right
            anchors.bottom: parent.bottom
            horizontalAlignment: Text.AlignHCenter
            font.pointSize: 12
            placeholderText: "Server password"
            anchors.bottomMargin: 60
            anchors.rightMargin: 110
            opacity: 0
            echoMode: TextInput.Password
            // maximumLength: 6
            Keys.onReturnPressed: internal.checkLogin()
            Keys.onEnterPressed: internal.checkLogin()
            Keys.onPressed: internal.resetTextLogin()

            Component.onCompleted: {
                imageAnimationRightMargin.running = true;
                imageAnimationOpacity.running = true;
                textFieldAnimationRightMargin.running = true;
                textFieldOpacity.running = true;
                // btnText.running = true;
                loginTextField.text = "";
                internal.resetTextLogin();
            }

            PropertyAnimation {
                id: textFieldAnimationRightMargin
                target: loginTextField
                property: "anchors.rightMargin"
                to: if(loginTextField.anchors.rightMargin === 110) return 30; else return 110
                duration: 5000
                easing.type: Easing.InOutQuint
            }

            PropertyAnimation {
                id: textFieldOpacity
                target: loginTextField
                property: "opacity"
                to: if(loginTextField.opacity === 0) return 1; else return 0
                duration: 5000
                easing.type: Easing.InOutQuint
            }
        }

        Label {
            id: labelPassword
            x: 311
            color: "#ff007f"
            text: qsTr("Incorrect password 😥")
            anchors.right: parent.right
            anchors.top: loginTextField.bottom
            horizontalAlignment: Text.AlignRight
            anchors.topMargin: 0
            anchors.rightMargin: 30
            font.pointSize: 10
            font.weight: Font.Normal
            font.family: "Segoe UI"
            font.bold: false
            visible: false
            antialiasing: false
        }
    }

    DropShadow {
        id: dropShadowBG
        opacity: 0
        anchors.fill: bg
        source: bg
        verticalOffset: 0
        horizontalOffset: 0
        radius: 10
        color: "#40000000"
        z: 1
    }

    DropShadow {
        id: dropShadowLogin
        visible: false
        anchors.fill: loginFrame
        source: loginFrame
        verticalOffset: 0
        horizontalOffset: 0
        radius: 10
        color: "#40000000"
        z: 0
    }

    Timeline {
        id: timeline
        animations: [
            TimelineAnimation {
                id: timelineAnimation
                running: true
                loops: 1
                duration: 3000
                to: 3000
                from: 0
            }
        ]

        endFrame: 3000
        enabled: true
        startFrame: 0

        KeyframeGroup {
            target: circularProgressBar
            property: "value"
            
            Keyframe {
                frame: 0
                value: 0
            }

            Keyframe {
                frame: 1505
                value: 100
            }
        }

        KeyframeGroup {
            target: circularProgressBar
            property: "opacity"

            Keyframe {
                frame: 1505
                value: 1
            }

            Keyframe {
                frame: 1902
                value: 0
            }

            Keyframe {
                frame: 0
                value: 1
            }

            Keyframe {
                frame: 250
                value: 1
            }
        }

        KeyframeGroup {
            target: btnClose
            property: "opacity"

            Keyframe {
                frame: 1501
                value: 0
            }

            Keyframe {
                frame: 2000
                value: 1
            }

            Keyframe {
                frame: 0
                value: 0
            }
        }

        KeyframeGroup {
            target: logoImage
            property: "opacity"

            Keyframe {
                frame: 1750
                value: 0
            }

            Keyframe {
                frame: 2250
                value: 1
            }

            Keyframe {
                frame: 0
                value: 0
            }
        }

        KeyframeGroup {
            target: bg
            property: "anchors.verticalCenterOffset"

            Keyframe {
                frame: 1998
                value: 0
            }

            Keyframe {
                easing.bezierCurve: [0.254, 0.00129, 0.235, 0.999, 1, 1]
                frame: 2697
                value: -80
            }

            Keyframe {
                frame: 0
                value: 0
            }
        }

        KeyframeGroup {
            target: loginFrame
            property: "anchors.topMargin"

            Keyframe {
                easing.bezierCurve: [0.254, 0.00129, 0.235, 0.999, 1, 1]
                frame: 2700
                value: -20
            }

            Keyframe {
                frame: 2000
                value: -280
            }

            Keyframe {
                frame: 0
                value: -280
            }
        }

        KeyframeGroup {
            target: labelUnlockInfo1
            property: "opacity"

            Keyframe {
                frame: 2000
                value: 1
            }

            Keyframe {
                frame: 1500
                value: 0
            }

            Keyframe {
                frame: 0
                value: 0
            }
        }

        // KeyframeGroup {
        //     target: btnChangeUser
        //     property: "opacity"

        //     Keyframe {
        //         frame: 1900
        //         value: 0
        //     }

        //     Keyframe {
        //         frame: 2450
        //         value: 1
        //     }

        //     Keyframe {
        //         frame: 0
        //         value: 0
        //     }
        // }

        KeyframeGroup {
            target: bg
            property: "opacity"

            Keyframe {
                frame: 250
                value: 1
            }

            Keyframe {
                frame: 0
                value: 0
            }
        }

        KeyframeGroup {
            target: dropShadowBG
            property: "opacity"

            Keyframe {
                frame: 250
                value: 1
            }

            Keyframe {
                frame: 0
                value: 0
            }
        }

        KeyframeGroup {
            target: dropShadowLogin
            property: "visible"

            Keyframe {
                frame: 497
                value: false
            }

            Keyframe {
                frame: 550
                value: true
            }

            Keyframe {
                frame: 0
                value: false
            }
        }

        KeyframeGroup {
            target: loginFrame
            property: "visible"

            Keyframe {
                frame: 497
                value: false
            }

            Keyframe {
                frame: 550
                value: true
            }

            Keyframe {
                frame: 0
                value: false
            }
        }

        KeyframeGroup {
            target: imageFingerPrint
            property: "opacity"

            Keyframe {
                frame: 2500
                value: 0
            }

            Keyframe {
                frame: 2950
                value: 1
            }

            Keyframe {
                frame: 0
                value: 0
            }
        }

        // KeyframeGroup {
        //     target: image1
        //     property: "opacity"

        //     Keyframe {
        //         frame: 1450
        //         value: 1
        //     }

        //     Keyframe {
        //         frame: 1750
        //         value: 0
        //     }

        //     Keyframe {
        //         frame: 500
        //         value: 1
        //     }

        //     Keyframe {
        //         frame: 252
        //         value: 1
        //     }
        // }

        KeyframeGroup {
            target: labelName
            property: "anchors.bottomMargin"
            
            Keyframe {
                easing.bezierCurve: [0.254, 0.00129, 0.235, 0.999, 1, 1]
                value: 22
                frame: 2350
            }

            Keyframe {
                value: -25
                frame: 1798
            }

            Keyframe {
                value: -250
                frame: 0
            }
        }
    }

    Connections {
        target: backend
    }
}
