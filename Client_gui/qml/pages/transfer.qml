import QtQuick 2.0
import "../components"
import QtQuick.Timeline 1.0
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import "../QmlTreeWidget"

Item {
    Rectangle {
        anchors.fill: parent
        radius: 10
        color: "#27273a"

        GridLayout {
            id: gridLayout
            columns: 3
            rows: 2
            anchors.fill: parent
            anchors.margins: 30
            columnSpacing: 10
            rowSpacing: 50
            Column {
                Row {
                    Label {
                        id: userPc
                        color: "#ffffff"
                        text: qsTr("User PC")
                        font.family: "Segoe UI"
                        font.pointSize: 11
                    }
                }

                Row {
                    TreeWidget {
                        id: tree
                        //                        anchors.fill: parent

                        Component.onCompleted: {
                            iconSize = (Qt.size(12, 12));
                            font.family = "Monaco";
                            font.pointSize = 16;

                            var C = tree.createItem("C");
                            var D = tree.createItem("D");

                            C.appendChild(tree.createItem("Program Files"));
                            C.appendChild(tree.createItem("Program Files (x86)"));
                            C.appendChild(tree.createItem("Users"));
                            C.appendChild(tree.createItem("Windows"));
                            tree.addTopLevelItem(C);
                            tree.addTopLevelItem(D);
                        }
                    }

                    //                    Button{
                    //                        id: buttonRename
                    //                        text: "Rename"

                    //                        anchors.left: parent.left;
                    //                        anchors.bottom: parent.bottom;

                    //                        onClicked: {
                    //                            var curItem = tree.getCurrentItem();
                    //                            if(curItem){
                    //                                curItem.setText("New Name");
                    //                            }
                    //                        }
                }
            }
            Column {
                Row {
                    Label {
                        id: serverPc
                        color: "#ffffff"
                        text: qsTr("Server PC")
                        font.family: "Segoe UI"
                        font.pointSize: 11
                    }
                }

                Row {
                    TreeWidget {
                        id: tree2
                        //                        anchors.fill: parent

                        Component.onCompleted: {
                            iconSize = (Qt.size(12, 12));
                            font.family = "Monaco";
                            font.pointSize = 16;

                            var C = tree.createItem("C");
                            var D = tree.createItem("D");

                            C.appendChild(tree.createItem("Program Files"));
                            C.appendChild(tree.createItem("Program Files (x86)"));
                            C.appendChild(tree.createItem("Users"));
                            C.appendChild(tree.createItem("Windows"));
                            tree.addTopLevelItem(C);
                            tree.addTopLevelItem(D);
                        }
                    }

                    //                    Button{
                    //                        id: buttonRename
                    //                        text: "Rename"

                    //                        anchors.left: parent.left;
                    //                        anchors.bottom: parent.bottom;

                    //                        onClicked: {
                    //                            var curItem = tree.getCurrentItem();
                    //                            if(curItem){
                    //                                curItem.setText("New Name");
                    //                            }
                    //                        }
                }
            }
        }
    }
}

/*##^##
Designer {
    D{i:0;autoSize:true;formeditorZoom:1.33;height:480;width:640}
}
##^##*/
