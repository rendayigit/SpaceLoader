import QtQuick 2.15
import "../components"
import QtQuick.Timeline 1.0
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

Item {
    id: item1


    property var loglist: []

    Component.onCompleted: {
        backend.listLogs()
    }

    Rectangle {
        id: rectangle
        anchors.fill: parent
        radius: 10
        color: "#27273a"

        CustomTextField {
            id: searchInput
            
            anchors.left: parent.left
            anchors.top: parent.top
            anchors.leftMargin: 5
            anchors.topMargin: 5
            
            height: 40
            width: 248
            
            placeholderText: "Search"
            
            font.pointSize: 12
            Keys.onReleased: {
                var searchedComponents = []

                for(var i = 0 ; i < loglist.length; i++) {
                    if (loglist[i].text.startsWith(searchInput.text)) {
                        searchedComponents.push(loglist[i].text);
                    }
                }

                for(var i = 0; i < logsColumn.children.length; i++) {
                    if(searchedComponents.includes(logsColumn.children[i].text)){
                        logsColumn.children[i].visible = true;
                    }else{
                        logsColumn.children[i].visible = false;
                    }
                }
            }            
        }

        Rectangle {
            id: leftMenu
            width: 240
            color: "#00000000"
            border.color: "#00000000"
            border.width: 0
            anchors.left: parent.left
            anchors.top: searchInput.bottom
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 10
            anchors.leftMargin: 5
            anchors.topMargin: 5

            ScrollView {
                anchors.left: parent.left
                anchors.bottom: parent.bottom
                anchors.top: parent.top
                anchors.leftMargin: 4
                anchors.bottomMargin: 4
                anchors.topMargin: 4
                clip: true

                Column {
                    id: logsColumn
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

        CustomTextField {
            id: searchText
            
            anchors.left: searchInput.right
            anchors.top: parent.top
            anchors.leftMargin: 5
            anchors.topMargin: 5
            
            height: 40
            width: 600
            
            placeholderText: "Search Log"

            Keys.onReleased: {
                var textList = logDisplay.text.split("\n")
                textList.shift()
                textList.shift()
                textList.shift()
                textList.shift()
                textList.shift()

                var colorList = textList.map((text) => {
                    return text.match("color:" + "(.*)" + ";")[1]
                })

                textList = textList.map((text) => {
                    var temp = text.match("<span" + "(.*)" + "/span>")[1]
                    return temp.match(">" + "(.*)" + "<")[1]
                })

                var color
                selectFile.visible = false
                logText.visible = true
                logDisplay.text = ""

                for (var i = 0; i < textList.length; i++) {
                    var searchList = textList[i].split(searchText.text)

                    if(textList[i].indexOf(searchText.text) == 0) {
                        logDisplay.text += "<font color='#ff0101'>" + searchText.text + "</font>"
                    }
                    // Exception is here
                    // for(var j = 0; j < searchList.length; j++) {
                    //     logDisplay.text += "<font color='" + colorList[i] + "'>" + searchList[j] + "</font>"
                    //     if(j != searchList.length -1) {
                    //         logDisplay.text += "<font color='#ff0101'>" + searchText.text + "</font>"
                    //     }
                    // }
                    if(textList[i].lastIndexOf(searchText.text) == textList[i].length - searchText.text.length - 1) {
                        logDisplay.text += "<font color='#ff0101'>" + searchText.text + "</font>"
                    }
                    logDisplay.text += "\n"
                }

                logDisplay.cursorPosition += logDisplay.length
            }
        }

        Rectangle {
            id: logText
            anchors.top: searchText.bottom
            anchors.bottom: parent.bottom
            anchors.left: leftMenu.right
            anchors.right: parent.right
            anchors.topMargin: 10
            anchors.bottomMargin: 10
            anchors.leftMargin: 10
            anchors.rightMargin: 10
            visible: false

            radius: 10
            color: "#000000"

            ScrollView {
                id: view
                anchors.fill: parent

                TextArea {
                    id: logDisplay
                    anchors.fill: parent
                    color: "#ffffff"
                    font.family: "Segoe UI"
                    font.pointSize: 11
                    anchors.centerIn: parent
                    textFormat: TextEdit.RichText
                }
            }
        }
    }

    Connections {
        target: backend

        function onClearLogs() {
            for(var i = 0 ; i < logsColumn.children.length; i++) {
                logsColumn.children[i].destroy()
            }

            logText.visible = false
            selectFile.visible = true
            logDisplay.text = ""
        }

        function onGetLogList(text) {
            var logItem = Qt.createComponent("../components/LogButton.qml")
                        .createObject(logsColumn, {
                                        "text": text,
                                        "btnIconSource": "../../assets/images/logs.png",
                                        "Layout.alignment": Qt.AlignHCenter | Qt.AlignVCenter
                                    });
            loglist.push(logItem)
        }

        function onGetLogText(text) {
            selectFile.visible = false
            logText.visible = true
            logDisplay.text = ""

            var textList = text.split("\n")
            var color
            for (var i = 0; i < textList.length; i++) {
                if(textList[i].includes("FATAL")) { color = "red" }
                else if(textList[i].includes("ERROR")) { color = "#ff0000" }
                else if(textList[i].includes("WARN")) { color = "#f5ad42" }
                else if(textList[i].includes("INFO")) { color = "#adf542" }
                else if(textList[i].includes("DEBUG")) { color = "#428df5" }
                else if(textList[i].includes("TRACE")) { color = "#c242f5" }
                else { color = "#ffffff" }
                logDisplay.text += "<font color='" + color + "'>" + textList[i] + "</font>\n"
            }

            logDisplay.cursorPosition += logDisplay.length
        }
    }
}
