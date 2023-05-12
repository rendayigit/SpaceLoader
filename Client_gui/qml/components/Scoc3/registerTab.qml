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

    signal registerClicked(string registerId);

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
        //console.log(registerId)
        backend.setGlobalFieldId(-1)
        console.log(moduleId)
        backend.setGlobalModuleId(parseInt(moduleId))
        clearConf()
        createFieldButtons(registerId)
    }

    Button {
        width: 20
        height: 20

        anchors.right: parent.right

        text: "X"

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
            destroyRegisterTabAlias(registerId, moduleId)
        }
    }
}
