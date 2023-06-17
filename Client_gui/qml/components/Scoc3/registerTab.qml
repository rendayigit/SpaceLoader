import QtQuick 2.15
import QtQuick.Window 2.15
import QtGraphicalEffects 1.15
import QtQuick.Timeline 1.0
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

Button {
    property string registerId;
    property string moduleId;
    property bool alert;

    width: 150
    height: 20
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
        backend.setGlobalFieldId(-1)
        backend.setGlobalModuleId(parseInt(moduleId))
        clearConf()
        createFieldButtons(registerId)
    }

    ToolTip.delay: 500
    ToolTip.timeout: 5000
    ToolTip.visible: hovered

    Component.onCompleted: {
        ToolTip.text = qsTr(backend.getFileList()[moduleId].split(".")[0] + " > " + backend.getRegisterList()[registerId])
    }

    Button {
        width: 20
        height: 20

        anchors.right: parent.right

//        text: "X"
        Image {
            source: "../../../assets/svg_images/close-outline.svg"
            width: parent.width
            height: parent.height
            anchors.centerIn: parent
        }

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
            destroyRegisterTabAlias(moduleId, registerId)
        }
    }
}
