import QtQuick 2.0
import "../components"
import QtQuick.Timeline 1.0
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

Item {
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
            text: qsTr("OBSW Image:")
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

            GridLayout {
                id: gridLayout
                columns: 3
                anchors.centerIn: parent
                columnSpacing: 15

                Row {
                    Image {
                        source: "../../assets/images/upload.png"
                    }
                }

                Row {
                    Label {
                        id: uploadAreaLabel
                        color: "#ffffff"
                        text: qsTr("Upload image here or")
                        font.family: "Segoe UI"
                        font.pointSize: 11
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
                    }
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
