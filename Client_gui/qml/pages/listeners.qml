import QtQuick 2.0
import "../components"
import QtQuick.Timeline 1.0
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

Item {
    id: item1

    Rectangle {
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

                        LeftButton {
                            colorDefault: "#40405f"
                            text: "192.168.1.2:8080.log"
                            btnIconSource: "../../assets/images/connect.png"
                            //                            onClicked: color= "#ffffff"
                            Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                        }

                        LeftButton {
                            text: "192.168.1.2:8081.log"
                            btnIconSource: "../../assets/images/connect.png"
                            //                            onClicked: stackView.push("pageNoInternet.qml")
                            Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
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
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            anchors.left: leftMenu.right
            anchors.right: parent.right
            anchors.topMargin: 10
            anchors.bottomMargin: 10
            anchors.leftMargin: 10
            anchors.rightMargin: 10

            radius: 10
            color: "#000000"

            ScrollView {
                id: view
                anchors.fill: parent

                TextArea {
                    anchors.fill: parent
                    color: "#ffffff"
                    text: " Sun Apr 24 19:36:56 2022 INFO Server Started \n Sun Apr 24 19:36:56 2022 INFO Parsed ServerCmds.yaml \n Sun Apr 24 19:37:14 2022 INFO Received: 'addUser renda' from 127.0.0.1 \n Sun Apr 24 19:37:14 2022 EVENT renda (127.0.0.1) connected. \n Sun Apr 24 19:37:18 2022 INFO Received: 'getuserlist' from 127.0.0.1 \n Sun Apr 24 19:37:24 2022 INFO Received: 'getuserlist' from 127.0.0.1 \n Sun Apr 24 19:37:36 2022 INFO Received: 'sa' from 127.0.0.1 \n Sun Apr 24 19:37:38 2022 INFO Received: 'sa' from 127.0.0.1 \n Sun Apr 24 19:39:26 2022 INFO 127.0.0.1 disconnected \n Sun Apr 24 19:39:32 2022 INFO Server Started \n Sun Apr 24 19:39:32 2022 INFO Parsed ServerCmds.yaml \n Sun Apr 24 19:39:34 2022 INFO Received: 'addUser renda' from 127.0.0.1 \n Sun Apr 24 19:39:34 2022 EVENT renda (127.0.0.1) connected. \n Sun Apr 24 19:39:37 2022 INFO Received: 'getuserlist' from 127.0.0.1 \n Sun Apr 24 19:39:40 2022 INFO Received: 'sa' from 127.0.0.1 \n Sun Apr 24 19:39:50 2022 INFO Received: 'getauth sa' from 127.0.0.1 \n Sun Apr 24 19:39:54 2022 INFO Received: 'sa' from 127.0.0.1 \n Sun Apr 24 19:39:55 2022 INFO Received: 'sa' from 127.0.0.1 \n Sun Apr 24 19:40:21 2022 INFO Received: 'addUser renda' from 127.0.0.1 \n Sun Apr 24 19:40:21 2022 EVENT renda (127.0.0.1) connected. \n Sun Apr 24 19:40:23 2022 INFO Received: 'sa' from 127.0.0.1 \n Sun Apr 24 19:40:29 2022 INFO Received: 'sa' from 127.0.0.1 \n Sun Apr 24 19:40:34 2022 INFO Received: 'getuserlist' from 127.0.0.1 \n Sun Apr 24 19:41:50 2022 INFO 127.0.0.1 disconnected \n Sun Apr 24 19:42:03 2022 INFO Server Started \n Sun Apr 24 19:42:03 2022 INFO Parsed ServerCmds.yaml \n Sun Apr 24 19:42:05 2022 INFO Received: 'addUser renda' from 127.0.0.1 \n Sun Apr 24 19:42:05 2022 EVENT renda (127.0.0.1) connected. \n Sun Apr 24 19:42:06 2022 INFO Received: 'addUser renda' from 127.0.0.1 \n Sun Apr 24 19:42:06 2022 EVENT renda (127.0.0.1) connected. \n Sun Apr 24 19:42:09 2022 INFO Received: 'getuserlist' from 127.0.0.1 \n Sun Apr 24 20:19:47 2022 INFO Server Started \n Sun Apr 24 20:19:47 2022 INFO Parsed ServerCmds.yaml \n Sun Apr 24 20:19:50 2022 INFO Received: 'addUser renda' from 127.0.0.1 \n Sun Apr 24 20:19:50 2022 EVENT renda (127.0.0.1) connected. \n Sun Apr 24 20:19:59 2022 INFO Received: 'getuserlist' from 127.0.0.1 \n Sun Apr 24 20:20:03 2022 INFO Received: 'sa' from 127.0.0.1 \n Sun Apr 24 20:20:07 2022 INFO Received: 'getauth sa' from 127.0.0.1 \n Sun Apr 24 20:20:08 2022 INFO Received: 'sa' from 127.0.0.1 \n Sun Apr 24 20:20:09 2022 INFO Received: 'sa' from 127.0.0.1 \n Sun Apr 24 20:20:15 2022 INFO 127.0.0.1 disconnected \n Sun Apr 24 20:20:19 2022 INFO Received: 'addUser renda' from 127.0.0.1 \n Sun Apr 24 20:20:19 2022 EVENT renda (127.0.0.1) connected. \n Sun Apr 24 20:20:19 2022 INFO Received: 'sa' from 127.0.0.1 \n Sun Apr 24 20:20:21 2022 INFO Received: 'sa' from 127.0.0.1 \n Sun Apr 24 20:20:26 2022 INFO Received: 'getuserlist' from 127.0.0.1"
                    font.family: "Segoe UI"
                    font.pointSize: 11
                    anchors.centerIn: parent
                }
            }
        }
    }
}

/*##^##
Designer {
    D{i:0;autoSize:true;formeditorColor:"#c0c0c0";height:580;width:800}D{i:19;property:"opacity";target:"labelNoInternet"}
}
##^##*/
