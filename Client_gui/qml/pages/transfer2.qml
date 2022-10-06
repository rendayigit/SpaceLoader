import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Dialogs 1.3
import "../components"

// TODO Dont forget to implement this page

Item {
    Label {
        id: localPathLabel
        color: "#ffffff"
        text: qsTr("Path to local file: ")
        font.pointSize: 10
        font.family: "Segoe UI"
        font.weight: Font.Normal

        anchors.top: parent.top
        anchors.left: parent.left

        anchors.topMargin: 25
        anchors.leftMargin: 20
    }

    DropArea {
        id: dropArea
        height: 40

        anchors.top: parent.top
        anchors.left: localPathLabel.right
        anchors.right: browseLocal.left

        anchors.topMargin: 20
        anchors.leftMargin: 10
        anchors.rightMargin: 20

        onDropped: (drop) => {
            localPath.text = drop.text.split("file:///").join("")
        }
    }

    CustomTextField {
        id: localPath
        height: 40

        anchors.top: parent.top
        anchors.left: localPathLabel.right
        anchors.right: browseLocal.left

        anchors.topMargin: 20
        anchors.leftMargin: 10
        anchors.rightMargin: 20

        placeholderText: ""
        onEditingFinished: if(serverPath.text.length > 0 && localPath.text.length > 0) {
                            upload.enabled = true
                        } else {
                            upload.enabled = false
                        }

        font.pointSize: 12
    }
    
    
    CustomButton {
        id: browseLocal
        anchors.top: parent.top
        anchors.right: parent.right

        anchors.topMargin: 20
        anchors.rightMargin: 20

        text: "Browse"
        width: 100
        height: 40

        onClicked: fileDialog.visible = true
    }
     Label {
        id: serverPathLabel
        color: "#ffffff"
        text: qsTr("Path to copy file: ")
        font.pointSize: 10
        font.family: "Segoe UI"
        font.weight: Font.Normal

        anchors.top: localPathLabel.bottom
        anchors.left: parent.left

        anchors.topMargin: 40
        anchors.leftMargin: 20
    }

    CustomTextField {
        id: serverPath
        height: 40

        anchors.top: localPath.bottom
        anchors.left: serverPathLabel.right
        anchors.right: upload.left

        anchors.topMargin: 20
        anchors.leftMargin: 10
        anchors.rightMargin: 20

        text: "D:/"
        onEditingFinished: if(serverPath.text.length > 0 && localPath.text.length > 0) {
                               upload.enabled = true
                           } else {
                               upload.enabled = false
                           }

        font.pointSize: 12
    }

    CustomButton {
        id: upload
        anchors.top: browseLocal.bottom
        anchors.right: parent.right

        anchors.topMargin: 20
        anchors.rightMargin: 20

        text: "Upload"
        width: 100
        height: 40

        colorDefault: "#4632a8"
        colorMouseOver: "#5643b5"
        colorPressed: "#7963e6"

        enabled: false

        onClicked: if(serverPath.text.length > 0 && localPath.text.length > 0) {
                       backend.fileTransfer(localPath.text, serverPath.text)
                   } else {
                       print("error")
                   }
    }

    FileDialog {
        id: fileDialog
        title: "Please choose a file"
        folder: shortcuts.home
        onAccepted: localPath.text = fileDialog.fileUrl
        visible: false
    }
}

/*##^##
Designer {
    D{i:0;autoSize:true;formeditorZoom:1.33;height:480;width:640}
}
##^##*/
