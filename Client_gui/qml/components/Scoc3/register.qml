import QtQuick 2.15
import QtQuick.Window 2.15
import QtGraphicalEffects 1.15
import QtQuick.Timeline 1.0
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

Button {
    property string registerId;
    property bool alert;

    signal registerClicked(string registerId);

    width: rootObject.width / 6

    background: Rectangle {
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
        registerClicked(registerId)
    }

    Text {
        width: rootObject.width / 6
        height: 25
        color: "#ffb22d"
        text: "⚠  "
        font.bold: true
        font.pointSize: 12
        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignRight
        visible: alert
    }


    Button {
        anchors.right: parent.right

        background: Rectangle{
            color: "#4891d9"
            radius: 10
        }

        height: parent.height
        width: parent.height

        text: "-"

        onPressed: background.color = "#a3bed0"
        onReleased: { background.color = "#4891d9"
            if (hovered) {background.color = "#74a8db"}
            else {background.color = "#4891d9"}
        }

        onClicked: {
            console.writeline("small but 1 pressed")
        }
    }

    Button {
        anchors.left: parent.left

        background: Rectangle{
            color: "#4891d9"
            radius: 10
        }

        height: parent.height
        width: parent.height

        text: "+"
//        Image {
//            id: name
//            source: "../assets/svg_images/push-pin.svg"
//            width: parent.width
//            height: parent.height
//            anchors.centerIn: parent
//        }

        onPressed: background.color = "#a3bed0"
        onReleased: { background.color = "#4891d9"
            if (hovered) {background.color = "#74a8db"}
            else {background.color = "#4891d9"}
        }

        onClicked: {
            console.writeline("small but 2 pressed")
        }
    }

}
