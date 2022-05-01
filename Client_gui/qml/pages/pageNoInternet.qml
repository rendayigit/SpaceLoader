import QtQuick 2.0
import "../components"
import QtQuick.Timeline 1.0
import QtQuick.Controls 2.15

Item {
    id: item1

    Rectangle {
        id: rectangle
        anchors.fill: parent
        radius: 10
        color: "#27273a"

        CircularProgressBar {
            id: circularProgressBar
            width: 200
            height: 200
            opacity: 0
            text: "loading..."
            anchors.verticalCenter: parent.verticalCenter
            textShowValue: false
            anchors.horizontalCenter: parent.horizontalCenter
            value: 0
            textColor: "#ffffff"
            progressColor: "#55aaff"
            strokeBgWidth: 2
            enableDropShadow: false
            progressWidth: 4
            bgStrokeColor: "#33334c"
        }

        Label {
            id: label
            x: 348
            y: 215
            opacity: 1
            color: "#ffffff"
            text: qsTr("Page not yet constructed :)")
            anchors.verticalCenter: parent.verticalCenter
            anchors.verticalCenterOffset: 0
            font.bold: true
            font.pointSize: 16
            font.family: "Segoe UI"
            anchors.horizontalCenter: parent.horizontalCenter
        }
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
        enabled: true
        startFrame: 0
        endFrame: 3000

        KeyframeGroup {
            target: circularProgressBar
            property: "value"
            Keyframe {
                value: 100
                frame: 1397
            }

            Keyframe {
                value: 0
                frame: 0
            }
        }

        KeyframeGroup {
            target: circularProgressBar
            property: "opacity"
            Keyframe {
                value: 1
                frame: 1299
            }

            Keyframe {
                value: 0
                frame: 1601
            }
        }

        KeyframeGroup {
            target: label
            property: "opacity"
            Keyframe {
                value: 0
                frame: 1697
            }

            Keyframe {
                value: 1
                frame: 2198
            }

            Keyframe {
                value: 0
                frame: 0
            }
        }
    }
}

/*##^##
Designer {
    D{i:0;autoSize:true;formeditorColor:"#c0c0c0";height:580;width:800}D{i:19;property:"opacity";target:"labelNoInternet"}
D{i:6}
}
##^##*/
