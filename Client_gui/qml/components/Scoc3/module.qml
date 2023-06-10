import QtQuick 2.15
import QtQuick.Window 2.15
import QtGraphicalEffects 1.15
import QtQuick.Timeline 1.0
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

Button {
    property string moduleId;
    property bool alert;

    signal moduleClicked(string moduleId);

    width: rootObject.width / 6

    background: Rectangle {
        id: moduleButtonBackground
        color: "#4891d9"
        radius: 10
    }

    onPressed: background.color = "#a3bed0"
    onReleased: { background.color = "#4891d9"
        if (hovered) {background.color = "#74a8db"}
        else {background.color = "#4891d9"}
    }

    onHoveredChanged: {
        if (hovered) {background.color = "#74a8db"}
        else {background.color = "#4891d9"}
    }

    onClicked: {
        moduleClicked(moduleId)
    }

    Text {
        width: rootObject.width / 6
        height: 25
        text: "⚠  "
        font.pointSize: 12
        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignRight
        visible: alert
    }

    Button {
        anchors.left: parent.left

        background: Rectangle{
            color: "#4891d9"
            radius: 10
        }

        height: parent.height
        width: parent.height

        Image {
            id: svgdeneme
            source: "../../../assets/svg_images/push-pin-bold.svg"
            //source: "../../../assets/svg_images/push-pin-fill.svg"
            width: parent.width-13
            height: parent.height-13
            anchors.centerIn: parent
        }

        onPressed: background.color = "#a3bed0"
        onReleased: { background.color = "#4891d9"
            if (hovered) {background.color = "#74a8db"}
            else {background.color = "#4891d9"}
        }

        onHoveredChanged: {
            if (hovered) {
                background.color = "#74a8db"
                moduleButtonBackground.color = "#4891d9"
            }
            else {
                background.color = "#4891d9"
                if (parent.hovered) {
                    moduleButtonBackground.color = "#74a8db"
                }
            }
        }

        onClicked: {
            console.log("small but 2 pressed")
            backend.addToPinConfig("module", moduleId);
        }
    }
}
