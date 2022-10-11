import QtQuick 2.0
import "../components"
import QtQuick.Timeline 1.0
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtQuick.Dialogs 1.3

Item {
    property var fullFileText: qsTr("")

    Rectangle {
        anchors.fill: parent
        radius: 10
        color: "#27273a"

        Label {
            id: obswLabel
            anchors.left: parent.left
            anchors.top: parent.top
            anchors.leftMargin: 20
            anchors.topMargin: 20
            color: "#ffffff"
            text: qsTr("Local File:")
            font.bold: false
            font.family: "Segoe UI"
            font.pointSize: 11
        }

        Rectangle {
            id: uploadArea
            height: 150
            anchors.top: obswLabel.bottom
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.topMargin: 20
            anchors.leftMargin: 50
            anchors.rightMargin: 50
            radius: 10
            color: "#49496b"

            DropArea {
                id: dropArea
                height: 40

                anchors.fill: parent

                onDropped: (drop) => {
                    fullFileText = drop.text.split("file:///").join("")
                    uploadAreaLabel.text = ".../" + fullFileText.slice(-30)
                    showArea.visible = true
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
                        id: uploadAreaLabel
                        color: "#ffffff"
                        text: qsTr("Upload image here or")
                        font.family: "Segoe UI"
                        font.pointSize: 13
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
                        title: "Please choose a file"
                        folder: shortcuts.home
                        onAccepted: () => {
                            uploadAreaLabel.text = ".../" + fileDialog.fileUrl.toString().split("file:///").join("").slice(-30)
                            fullFileText = fileDialog.fileUrl.toString().split("file:///").join("")
                            uploadIcon.source = "../../assets/images/transfer.png"
                            showArea.visible = true
                        }
                        visible: false
                    }
                }
            }
        }

        Rectangle {
            id: showArea
            anchors.top: uploadArea.bottom
            anchors.horizontalCenter: parent.horizontalCenter
            width: uploadArea.width - 40

            anchors.topMargin: 20
            visible: false

            height: 80
            radius: 10
            color: "#414159"

            Label {
                id: serverPathLabel
                color: "#ffffff"
                text: qsTr("Remote Path: ")
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
                anchors.right: upload.left

                anchors.topMargin: 20
                anchors.leftMargin: 10
                anchors.rightMargin: 20

                text: "D:/"
                onEditingFinished: if(serverPath.text.length > 0 && fullFileText.text.length > 0) {
                                    upload.enabled = true
                                } else {
                                    upload.enabled = false
                                }

                font.pointSize: 12
            }

            CustomButton {
                id: upload
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

                enabled: false

                onClicked: if(serverPath.text.length > 0 && fullFileText.text.length > 0) {
                            backend.fileTransfer(localPath.text, serverPath.text)
                        } else {
                            print("error")
                        }
            }
        }
    }
}

/*##^##
Designer {
    D{i:0;autoSize:true;height:480;width:640}
}
##^##*/
