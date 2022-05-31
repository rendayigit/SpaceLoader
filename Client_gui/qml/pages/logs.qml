import QtQuick 2.15
import "../components"
import QtQuick.Timeline 1.0
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

Item {
    id: item1


    property var loglist: []
    property var displayLogText: ""
    property var cursorList: []
    property var cursorListIndex: 0

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

            Keys.onReleased:(event)=> {
                if (event.key === Qt.Key_Return || event.key === Qt.Key_Enter) {
                    logDisplay.cursorPosition = cursorList[cursorListIndex]
                    
                    if (event.key === Qt.Key_Return) {
                        if(cursorListIndex != cursorList.length - 1) {
                            cursorListIndex += 1
                        } else {
                            cursorListIndex = 0
                        }
                    } else {
                        if(cursorListIndex == 0) {
                            cursorListIndex = cursorList.length - 1
                        } else {
                            cursorListIndex -= 1
                        }
                    }
                } else {
                    cursorList = []                                                                                                                                                                                                                                     
                    var count = 0

                    var textList = displayLogText.split("<br />")
                    textList.shift()
                    textList.shift()
                    textList.shift()
                    textList.shift()
                    textList.shift()
                    textList.pop()
                    textList.pop()
                
                    var colorList = textList.map((text) => {
                        return text.match("color" + "(.*)" + ";")[1].substring(1)
                    })

                    textList = textList.map((text) => {
                        var temp = text.match("<span" + "(.*)" + "/span>")[1]
                        return temp.match(">" + "(.*)" + "<")[1]
                    })

                    var color
                    selectFile.visible = false
                    logText.visible = true
                    var edittedText
                    var keyword = searchText.text

                    for (var i = 0; i < textList.length; i++) {
                        var searchList = textList[i].split(keyword)
                        if(textList[i].indexOf(keyword) == 0) {
                            edittedText += "<font color='#ffff28' font-weight=bold>" + keyword + "</font>"
                            cursorList.push(count)
                            count += keyword.length
                        }
                        for(var j = 0; j < searchList.length; j++) {
                            edittedText += "<font color='" + colorList[i] + "'>" + searchList[j] + "</font>"
                            count += searchList[j].length
                            if(j != searchList.length -1 && searchList[j] != "") {
                                edittedText += "<font color='#ffff28' font-weight=bold>" + keyword + "</font>"
                                cursorList.push(count)
                                count += keyword.length
                            }
                        }
                        if(textList[i].lastIndexOf(keyword) == textList[i].length - keyword.length - 1) {
                            edittedText += "<font color='#ffff28'>" + keyword + "</font>"
                            cursorList.push(count)
                            count += keyword.length
                        }
                        edittedText += "<br>"
                    }
                    logDisplay.text = edittedText
                }
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
            var edittedText
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
                edittedText += "<font color='" + color + "'>" + textList[i] + "</font><br>"
            }

            logDisplay.text = edittedText
            displayLogText = logDisplay.text
            logDisplay.cursorPosition += logDisplay.length
        }
    }
}
