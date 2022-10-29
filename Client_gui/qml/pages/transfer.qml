import QtQuick 2.0
import QtQuick.Timeline 1.0
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtQuick.Dialogs 1.3
import "../components"

Rectangle {
    id: root

    property string fileToUploadPath: ""

    radius: 10
    color: "#27273a"

    Rectangle {
        id: remoteArea
        
        anchors.top: uploadArea.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.topMargin: - 80

        height: 80
        width: uploadArea.width - 40
        
        radius: 10
        color: "#414159"

        Label {
            id: serverPathLabel
            
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.topMargin: 30
            anchors.leftMargin: 20
            
            color: "#ffffff"
            font.pointSize: 10
            font.family: "Segoe UI"
            font.weight: Font.Normal

            text: "Remote Path: "
        }

        CustomTextField {
            id: serverPath

            anchors.top: parent.top
            anchors.left: serverPathLabel.right
            anchors.right: buttonUpload.left
            anchors.topMargin: 20
            anchors.leftMargin: 10
            anchors.rightMargin: 20

            height: 40
            font.pointSize: 12

            text: "D:/"
        }

        CustomButton {
            id: buttonUpload
            
            anchors.top: parent.top
            anchors.right: parent.right
            anchors.topMargin: 20
            anchors.rightMargin: 20

            width: 100
            height: 40

            colorDefault: "#4632a8"
            colorMouseOver: "#5643b5"
            colorPressed: "#7963e6"
            
            text: "Upload"

            onClicked: {
                print("file to upload: " + fileToUploadPath)
                print("server path: " + serverPath.text)

                if (serverPath.text.length > 0 && fileToUploadPath.length > 0) {
                    // TODO - implement
                    // backend.changeYamlFile("Config.FileTransfer.remotePath", serverPath.text)
                    // backend.fileTransfer(fileToUploadPath, serverPath.text)
                } else {
                    print("Upload Error, Set server path and select a file to upload first.")
                }
            }
        }

        PropertyAnimation {
            id: remoteAreaAnimation

            target: remoteArea
            property: "anchors.topMargin"
            to: 20;
            duration: 5000
            easing.type: Easing.OutQuint
        }
    }

    Rectangle {
        id: uploadArea

        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.topMargin: 30
        anchors.leftMargin: 50
        anchors.rightMargin: 50
        
        height: 150
        radius: 10
        color: "#49496b"

        DropArea {
            id: dropArea

            anchors.fill: parent

            onDropped: root.setReadyForUpload(drop.text)
        }

        GridLayout {
            id: gridLayout
            
            anchors.centerIn: parent
            
            columns: 3
            columnSpacing: 20

            Row {
                Image {
                    id: uploadIcon

                    source: "../../assets/images/upload.png"
                }
            }

            Row {
                Label {
                    id: labelUploadArea
                    
                    anchors.verticalCenter: parent.verticalCenter
                    
                    color: "#ffffff"
                    font.family: "Segoe UI"
                    font.pointSize: 13
                    wrapMode: Label.WordWrap
                    
                    text: "Upload image here or"
                }
            }

            Row {
                CustomButton {
                    width: 108
                    height: 30
                    
                    colorMouseOver: "#40405f"
                    colorDefault: "#33334c"
                    colorPressed: "#55aaff"
                    
                    text: "Browse"

                    onClicked: fileDialog.visible = true
                }

                FileDialog {
                    id: fileDialog
                    
                    title: "Select a file to upload"
                    folder: shortcuts.home
                    visible: false

                    onAccepted: root.setReadyForUpload(fileDialog.fileUrl.toString())
                }
            }
        }
    }

    function setReadyForUpload(path) {
        fileToUploadPath = path.split("file:///").join("")
        labelUploadArea.text = ".../" + fileToUploadPath.slice(-30)
        uploadIcon.source = "../../assets/images/transfer.png"
        remoteAreaAnimation.running = true;
    }

    Connections {
        target: backend
    }
}
