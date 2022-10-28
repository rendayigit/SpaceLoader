import QtQuick 2.0
import QtQuick.Timeline 1.0
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtQuick.Dialogs 1.3
import "../components"

Item {
    property string fullFileText: ""

    Rectangle {
        anchors.fill: parent
        radius: 10
        color: "#27273a"

        Rectangle {
            id: uploadArea
            height: 150
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.topMargin: 30
            anchors.leftMargin: 50
            anchors.rightMargin: 50
            radius: 10
            color: "#49496b"

            DropArea {
                id: dropArea
                anchors.fill: parent

                onDropped: (drop) => {
                               fullFileText = drop.text.split("file:///").join("")
                               labelUploadArea.text = ".../" + fullFileText.slice(-30)
                               remoteArea.visible = true
                               uploadIcon.source = "../../assets/images/transfer.png"
                               // uploadIcon.destroy()
                           }
            }

            GridLayout {
                id: gridLayout
                columns: 3
                anchors.centerIn: parent
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
                        color: "#ffffff"
                        text: "Upload image here or"
                        font.family: "Segoe UI"
                        font.pointSize: 13
                        anchors.verticalCenter: parent.verticalCenter
                        wrapMode: Label.WordWrap
                    }
                }

                Row {
                    CustomButton {
                        width: 108
                        height: 30
                        text: "Browse"
                        colorMouseOver: "#40405f"
                        colorDefault: "#33334c"
                        colorPressed: "#55aaff"

                        onClicked: fileDialog.visible = true
                    }

                    FileDialog {
                        id: fileDialog
                        title: "Please choose a file to upload"
                        folder: shortcuts.home
                        visible: false

                        onAccepted: () => {
                                        labelUploadArea.text = ".../" + fileDialog.fileUrl.toString().split("file:///").join("").slice(-30)
                                        fullFileText = fileDialog.fileUrl.toString().split("file:///").join("")
                                        uploadIcon.source = "../../assets/images/transfer.png"
                                        remoteArea.visible = true
                                    }
                    }
                }
            }
        }

        Rectangle {
            id: remoteArea
            anchors.top: uploadArea.bottom
            anchors.horizontalCenter: parent.horizontalCenter

            height: 80
            width: uploadArea.width - 40
            radius: 10

            anchors.topMargin: 20
            visible: false

            color: "#414159"

            Label {
                id: serverPathLabel
                color: "#ffffff"
                text: "Remote Path: "
                font.pointSize: 10
                font.family: "Segoe UI"
                font.weight: Font.Normal

                anchors.top: parent.top
                anchors.left: parent.left

                anchors.topMargin: 30
                anchors.leftMargin: 20
            }

            CustomTextField {
                id: serverPath
                height: 40

                anchors.top: parent.top
                anchors.left: serverPathLabel.right
                anchors.right: buttonUpload.left

                anchors.topMargin: 20
                anchors.leftMargin: 10
                anchors.rightMargin: 20

                text: "D:/"

                font.pointSize: 12
            }

            CustomButton {
                id: buttonUpload
                anchors.top: parent.top
                anchors.right: parent.right

                anchors.topMargin: 20
                anchors.rightMargin: 20

                text: "Upload"
                width: 100
                height: 40

                colorDefault: "#4632a8"
                colorMouseOver: "#5643b5"
                colorPressed: "#7963e6"

                onClicked: {
                    if(serverPath.text.length > 0 && fullFileText.length > 0) {
                        backend.changeYamlFile("Config.FileTransfer.remotePath", serverPath.text)
                        backend.fileTransfer(fullFileText, serverPath.text)
                    } else {
                        print("error")
                    }
                }
            }
        }
    }
}
