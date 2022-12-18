import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Shapes 1.15
import QtGraphicalEffects 1.15

ProgressBar {
    id: progressBar

    property color backColor: "#7963e6"
    property color color1: "#7963e6"
    property color color2: "#4632a8"
    property int progessorSize: 50

    indeterminate: true

    background: Rectangle {
        implicitWidth: 200
        implicitHeight: 6
        radius: 10
        color: backColor
    }

    contentItem: Item {
        anchors.fill: parent
        anchors.margins: 1
        visible: parent.indeterminate
        clip: true

        Row {
            Repeater {
                Rectangle {
                    color: index % 2 ? color1 : color2
                    width: progessorSize
                    height: progressBar.height
                }

                model: progressBar.width / progessorSize + 2
            }

            XAnimator on x {
                from: 0
                to: - progessorSize * 2
                loops: Animation.Infinite
                running: progressBar.indeterminate
            }
        }
    }
}
