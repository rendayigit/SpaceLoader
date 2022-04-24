import QtQuick 2.0
import "../components"
import QtQuick.Timeline 1.0
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

Item {
    id: item1

    Rectangle{
        id: rectangle
        anchors.fill: parent
        radius: 10
        color: "#27273a"

        Rectangle {
            id: leftMenu
            width: 240
            color: "#00000000"
            border.color: "#00000000"
            border.width: 0
            anchors.left: parent.left
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            clip: true
            anchors.bottomMargin: 10
            anchors.leftMargin: 5
            anchors.topMargin: 10

            Flickable {
                id: flickable
                width: 106
                contentHeight: 19 * 10 //(the scrollbar size) do amount of content * by 10 maybe
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.bottom: parent.bottom
                anchors.top: parent.top
                anchors.rightMargin: 4
                anchors.leftMargin: 4
                anchors.bottomMargin: 4
                anchors.topMargin: 4
                clip: true

                GridLayout {
                    id: gridLayoutBottom
                    rows: 19
                    anchors.leftMargin: 0
                    anchors.rightMargin: 0
                    anchors.topMargin: 0
                    anchors.bottomMargin: 0
                    rowSpacing: 1
                    columns: 0

                    Column {
                        id: rowMenus
                        anchors.left: parent.left
                        anchors.right: parent.right
                        anchors.top: parent.top
                        anchors.bottom: parent.bottom

                        LeftButton {
                            text: "14.06.2021.log"
                            btnIconSource: "../../assets/images/logs.png"
                            onClicked: stackView.push("pageNoInternet.qml")
                            Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                        }

                        LeftButton {
                            text: "13.06.2021.log"
                            btnIconSource: "../../assets/images/logs.png"
                            onClicked: stackView.push("pageNoInternet.qml")
                            Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                        }

                        LeftButton {
                            text: "12.06.2021.log"
                            btnIconSource: "../../assets/images/logs.png"
                            onClicked: stackView.push("pageNoInternet.qml")
                        }

                        LeftButton {
                            text: "11.06.2021.log"
                            btnIconSource: "../../assets/images/logs.png"
                            onClicked: stackView.push("pageNoInternet.qml")
                        }

                        LeftButton {
                            text: "10.06.2021.log"
                            btnIconSource: "../../assets/images/logs.png"
                            onClicked: stackView.push("pageNoInternet.qml")
                        }

                        LeftButton {
                            text: "09.06.2021.log"
                            btnIconSource: "../../assets/images/logs.png"
                            onClicked: stackView.push("pageNoInternet.qml")
                        }

                        LeftButton {
                            text: "09.06.2021.log"
                            btnIconSource: "../../assets/images/logs.png"
                            onClicked: stackView.push("pageNoInternet.qml")
                        }

                        LeftButton {
                            text: "09.06.2021.log"
                            btnIconSource: "../../assets/images/logs.png"
                            onClicked: stackView.push("pageNoInternet.qml")
                        }

                        LeftButton {
                            text: "09.06.2021.log"
                            btnIconSource: "../../assets/images/logs.png"
                            onClicked: stackView.push("pageNoInternet.qml")
                        }

                        LeftButton {
                            text: "09.06.2021.log"
                            btnIconSource: "../../assets/images/logs.png"
                            onClicked: stackView.push("pageNoInternet.qml")
                        }

                        LeftButton {
                            text: "09.06.2021.log"
                            btnIconSource: "../../assets/images/logs.png"
                            onClicked: stackView.push("pageNoInternet.qml")
                        }

                        LeftButton {
                            text: "09.06.2021.log"
                            btnIconSource: "../../assets/images/logs.png"
                            onClicked: stackView.push("pageNoInternet.qml")
                        }

                        LeftButton {
                            text: "09.06.2021.log"
                            btnIconSource: "../../assets/images/logs.png"
                            onClicked: stackView.push("pageNoInternet.qml")
                        }

                        LeftButton {
                            text: "09.06.2021.log"
                            btnIconSource: "../../assets/images/logs.png"
                            onClicked: stackView.push("pageNoInternet.qml")
                        }

                        LeftButton {
                            text: "09.06.2021.log"
                            btnIconSource: "../../assets/images/logs.png"
                            onClicked: stackView.push("pageNoInternet.qml")
                        }

                        LeftButton {
                            text: "09.06.2021.log"
                            btnIconSource: "../../assets/images/logs.png"
                            onClicked: stackView.push("pageNoInternet.qml")
                        }

                        LeftButton {
                            text: "09.06.2021.log"
                            btnIconSource: "../../assets/images/logs.png"
                            onClicked: stackView.push("pageNoInternet.qml")
                        }

                        LeftButton {
                            text: "09.06.2021.log"
                            btnIconSource: "../../assets/images/logs.png"
                            onClicked: stackView.push("pageNoInternet.qml")
                        }

                        LeftButton {
                            text: "09.06.2021.log"
                            btnIconSource: "../../assets/images/logs.png"
                            onClicked: stackView.push("pageNoInternet.qml")
                        }
                    }
                }

                ScrollBar.vertical: ScrollBar {
                    id: control
                    size: 0.3
                    position: 0.2
                    orientation: Qt.Vertical
                    visible: flickable.moving || flickable.moving

                    contentItem: Rectangle {
                        implicitWidth: 6
                        implicitHeight: 100
                        radius: width / 2
                        color: control.pressed ? "#55aaff" : "#40405f"
                    }
                }
            }
        }

        Rectangle {
            id: selectFile
            height: 150
            anchors.left: leftMenu.right
            anchors.right: parent.right
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            anchors.leftMargin: 150
            anchors.rightMargin: 150
            anchors.topMargin: 150
            anchors.bottomMargin: 150
            radius: 10
            color: "#49496b"

            Label {
                color: "#ffffff"
                text: qsTr("Select a log file from the left menu to display ..")
                font.family: "Segoe UI"
                font.pointSize: 11
                anchors.centerIn: parent
            }
        }
    }
}

/*##^##
Designer {
    D{i:0;autoSize:true;formeditorColor:"#c0c0c0";height:580;width:800}D{i:19;property:"opacity";target:"labelNoInternet"}
}
##^##*/
