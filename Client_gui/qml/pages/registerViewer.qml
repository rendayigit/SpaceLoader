import QtQuick 2.15
import QtQuick.Window 2.15
import QtGraphicalEffects 1.15
import QtQuick.Timeline 1.0
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import Qt.labs.folderlistmodel 2.1
import QtQuick.Dialogs 1.3
import "../components/Scoc3"

Rectangle {
    property int columnGap: 120
    property string targetName: "STARKIT" //this is the variable for the header

    id: rootObject
    anchors.fill: parent
    radius: 10
    color: "#27273a"

    Component.onCompleted: {
        backend.setDefaultConfigId("default.yaml")
    }

    AbstractDialog {
            id: configFileDialog
            width: 300
            height: 100

            Rectangle {
                id: newConfigRectangle
                color: "#27273a"

                Text {
                    id: newConfigCaption
                    anchors.top: newConfigRectangle.top
                    anchors.left: newConfigRectangle.left
                    anchors.margins: 15
                    color: "#ffffff"
                    text: "Enter the name of the new configuration:"
                    font.pointSize: 10
                }

                Row {
                    id: newConfigRow
                    anchors.top: newConfigCaption.bottom
                    anchors.left: newConfigRectangle.left
                    anchors.right: newConfigRectangle.right
                    anchors.margins: 15
                    height: 35
                    spacing: 10

                    TextField {
                        id: newConfigTextField
                        width: 200
                        height: 35
                        placeholderText: "Config Name"

                        onTextChanged: {
                            var confFileList = backend.getConfFileList()
                            for (var i=0; i<confFileList.length; i++){
                                if (text === confFileList[i]){
                                    configFileDialog.height = 130
                                    newConfigWarning.visible = true
                                    break
                                }
                                else {
                                    configFileDialog.height = 100
                                    newConfigWarning.visible = false
                                }
                            }
                        }
                    }

                    Button {
                        id: configFileDialogButton
                        text:"OK"
                        width: 55
                        height: 35
                        background: Rectangle {
                            color: "#4891d9"
                            radius: 10
                        }

                        onClicked: {
                            backend.checkAndSaveAll(newConfigTextField.text)
                            configContent.clear()
                            var configList = backend.getConfFileList()
                            for (var it in configList){
                                configContent.append({text:configList[it]})
                            }
                            backend.setDefaultConfigId(newConfigTextField.text)
                            configComboBox.currentIndex = backend.returnGlobalConfigId()
                            newConfigTextField.clear()
                            configFileDialog.close()
                        }
                    }
                }


                Text {
                    id: newConfigWarning
                    anchors.left: newConfigRectangle.left
                    anchors.bottom: newConfigRectangle.bottom
                    anchors.margins: 15
                    color: "#ff0000"
                    text: "File already exists, content will be overwritten."
                    font.pointSize: 10
                    visible: false
                }

            }
    }


    Row {
        id: confBar
        width: parent.width
        height: 50
        anchors.left: parent.left
        anchors.leftMargin: 20
        anchors.top: parent.top
        anchors.topMargin: 17
        anchors.right: parent.right
        anchors.rightMargin: 20
        spacing: 10

        Label {
            id: mainHeader
            
            color: "#ffffff"
            text: targetName + " Registers"
            font.pixelSize: 30
            font.family: "Segoe UI"
        }

        Text {
            text: "Reference Configurations"
            leftPadding: rootObject.width - mainHeader.width - configComboBox.width - refreshButton.width - saveAllButton.width - 6*confBar.spacing - 180
            topPadding: 11
            rightPadding: 10
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
            font.pointSize: 11
            color: "#FFFFFF"
        }

        ComboBox {
            id: configComboBox
            editable: true

            model: ListModel {
                id: configContent

                Component.onCompleted: {
                    configComboBox.currentIndex = backend.returnGlobalConfigId()
                }

            }

            Component.onCompleted: {
                var configList = backend.getConfFileList()
                for (var it in configList){
                    configContent.append({text:configList[it]})
                }
            }

            onCurrentValueChanged: {
                backend.setConfFilePath(currentIndex)
                createModuleButtons()

                if(!registerPlaceHolder.visible){
                    createRegisterButtons(backend.returnGlobalModuleId())
                    if(!fieldPlaceHolder.visible){
                        createFieldButtons(backend.returnGlobalRegId())
                        if(!confPlaceHolder.visible){
                            createConfScreen(backend.returnGlobalFieldId())
                        }
                    }
                }
            }

        }

        Button {
            id: refreshButton
            text: "Refresh"
            width: 90
            height: 30

            background: Rectangle {
                color: "#4891d9"
                radius: 10
            }
            onPressed: background.color = "#a3bed0"
            onReleased: { background.color = "#4891d9"
                if (hovered) {background.color = "#74a8db"}
                else {background.color = "#4891d9"}
            }

            onHoveredChanged: {
                if (hovered) {background.color = "#74a8db"}
                else {background.color = "#4891d9"}
            }

            onClicked: {
                refresh()
            }
        }

        Button {
            id: saveAllButton
            text: "Save All"
            width: 90
            height: 30
            background: Rectangle {
                color: "#4891d9"
                radius: 10
            }
            onPressed: background.color = "#a3bed0"
            onReleased: { background.color = "#4891d9"
                if (hovered) {background.color = "#74a8db"}
                else {background.color = "#4891d9"}
            }

            onHoveredChanged: {
                if (hovered) {background.color = "#74a8db"}
                else {background.color = "#4891d9"}
            }

            onClicked: {
                configFileDialog.open()
            }
        }

    }

    Row {
        id: topBar
        width: parent.width
        anchors.left: parent.left
        anchors.top: confBar.bottom
        anchors.right: parent.right
        anchors.margins: 4
        spacing: 4


        Text {
            text: "Modules"
            width: parent.width / 6
            horizontalAlignment: Text.AlignHCenter
            font.pointSize: 11
            color: "#FFFFFF"
        }

        Text {
            text: "Registers"
            width: parent.width / 6
            horizontalAlignment: Text.AlignHCenter
            font.pointSize: 11
            color: "#FFFFFF"
        }

//        Rectangle {
//            id: tryrect
//            width: (parent.width / 6) + (parent.width / 2)
//            color: "#FFFFFF"
//            anchors.right: parent.right

//            Text {
//                text: "Fields"
//                width: parent.width / 6
//                horizontalAlignment: Text.AlignHCenter
//                font.pointSize: 11
//                color: "#FFFFFF"
//            }

//            Text {
//                text: "Field Configuration"
//                width: parent.width / 2
//                horizontalAlignment: Text.AlignHCenter
//                font.pointSize: 11
//                color: "#FFFFFF"
//            }

//        }
        Rectangle {
                    width: (parent.width / 6) + (parent.width / 2)
                    height: 20
                    color: rootObject.color

                    Flickable {
                        id: tabFlick
                        width: parent.width
                        height: 20
                        clip: true

                        // Set the scroll direction to horizontal
                        contentWidth: registerTabRow.width
                        boundsBehavior: Flickable.StopAtBounds

                        // Define a row of buttons
                        Row {
                            id: registerTabRow
                            spacing: 1

                            property var buttonNameList: [];
                            property var buttonModuleList: [];

                            // Create multiple buttons with different text



                        }

//                        MouseArea {
//                                anchors.fill: parent
//                                hoverEnabled: true
//                                onWheel: {
//                                    if (wheel.angleDelta.y > 0) {
//                                        // Scroll left
//                                        tabFlick.flick(10,0)
//                                    } else if (wheel.angleDelta.y < 0) {
//                                        // Scroll right
//                                        tabFlick.flick(-10,0)
//                                    }
//                                }
//                            }
                    }

                }


    }

    ScrollView {
        id: moduleScrollView
        anchors.left: parent.left
        anchors.bottom: pinBoard.top
        anchors.top: topBar.bottom
        anchors.leftMargin: 4
        anchors.bottomMargin: 4
        anchors.topMargin: 4
        width: rootObject.width / 6
        clip: true

        Column {
            id: moduleColumn
            anchors.centerIn: parent
            spacing: 2
        }
    }

    ScrollView {
        id: registerScrollView
        anchors.left: moduleScrollView.right
        anchors.bottom: pinBoard.top
        anchors.top: topBar.bottom
        anchors.leftMargin: 4
        anchors.bottomMargin: 4
        anchors.topMargin: 4
        width: rootObject.width / 6
        clip: true

        Column {
            id: registerColumn
            anchors.centerIn: parent
            spacing: 2
        }
    }

    ScrollView {
        id: fieldScrollView
        anchors.left: registerScrollView.right
        anchors.bottom: pinBoard.top
        anchors.top: topBar.bottom
        anchors.leftMargin: 4
        anchors.bottomMargin: 4
        anchors.topMargin: 4
        width: rootObject.width / 6
        clip: true

        Column {
            id: fieldColumn
            anchors.centerIn: parent
            spacing: 2
        }
    }

    Rectangle {
        id: registerPlaceHolder
        anchors.left: moduleScrollView.right
        anchors.bottom: pinBoard.top
        anchors.top: topBar.bottom
        anchors.leftMargin: 4
        anchors.bottomMargin: 4
        anchors.topMargin: 4
        width: rootObject.width / 6
        clip: true
        color: "#4d4d63"
        radius: 10
        border.color: "#8f8fa8"

        Text{
            text: "Please select a module to list its registers."
            horizontalAlignment: Text.AlignHCenter
            color: "#FFFFFF"
            font.pointSize: 10
            anchors.centerIn: parent
            wrapMode: Text.Wrap
            width: parent.width - 16

        }
    }

    Rectangle {
        id: fieldPlaceHolder
        anchors.left: registerScrollView.right
        anchors.bottom: pinBoard.top
        anchors.top: topBar.bottom
        anchors.leftMargin: 4
        anchors.bottomMargin: 4
        anchors.topMargin: 4
        width: rootObject.width / 6
        clip: true
        color: "#4d4d63"
        radius: 10
        border.color: "#8f8fa8"

        Text{
            text: "Please select a register to list its fields."
            horizontalAlignment: Text.AlignHCenter
            color: "#FFFFFF"
            font.pointSize: 10
            anchors.centerIn: parent
            wrapMode: Text.Wrap
            width: parent.width - 16

        }
    }

//confscreen
    ScrollView {
        id: confScrollView
        anchors.left: fieldScrollView.right
        anchors.bottom: pinBoard.top
        anchors.top: topBar.bottom
        anchors.leftMargin: 4
        anchors.bottomMargin: 4
        anchors.topMargin: 4
        width: (rootObject.width / 2) - 20
        clip: true

        Column {
            id: confColumn
            anchors.fill: parent
            spacing: 2
        }
    }

    Rectangle {
        id: confPlaceHolder
        anchors.left: fieldScrollView.right
        anchors.bottom: pinBoard.top
        anchors.top: topBar.bottom
        anchors.leftMargin: 4
        anchors.bottomMargin: 4
        anchors.topMargin: 4
        width: (rootObject.width / 2) - 20
        clip: true
        color: "#4d4d63"
        radius: 10
        border.color: "#8f8fa8"

        Text{
            text: "Please select a field to open configuration menu."
            horizontalAlignment: Text.AlignHCenter
            color: "#FFFFFF"
            font.pointSize: 10
            anchors.centerIn: parent
            wrapMode: Text.Wrap
            width: parent.width - 16

        }
    }

    Rectangle {
        id: pinBoard
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.leftMargin: 4
        anchors.bottomMargin: 4
        anchors.topMargin: 4
        height: 73
        width: parent.width
        color: "#4d4d63"
        radius: 10
        border.color: "#8f8fa8"

        Rectangle {
            id: pinBoardHeader
            anchors.left: parent.left
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            width: 120
            height: parent.height

            color: "#4d4d63"
            radius: 10
            border.color: "#8f8fa8"

            //TRIAL AREA
            Component.onCompleted: {
                createPinButtons()



            }
            //TRIAL AREA

            Text {
                color: "#ffffff"
                anchors.centerIn: parent
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
                text: "Pin Board"
                font.pointSize: 12
            }
        }

        Flickable {
                id: flickablePinBoard
                width: parent.width
                height: parent.height
                anchors.left: pinBoardHeader.right
                anchors.right: parent.right
                anchors.top: parent.top
                anchors.bottom: parent.bottom
                anchors.topMargin: 5
                anchors.leftMargin: 5
                anchors.rightMargin: 5

                contentWidth: column.width
                contentHeight: column.height
                flickableDirection: Flickable.HorizontalFlick
                boundsBehavior: Flickable.StopAtBounds
                clip: true

                Column {
                    id: column
                    spacing: 5
                    height: parent.height

                    Row {
                        spacing: 5
                        id: pinButtonRow0
                    }

                    Row {
                        spacing: 5
                        id: pinButtonRow1
                    }
                }

            }

        Rectangle {
            id: pinBoardPlaceHolder
            anchors.verticalCenter: pinBoard.verticalCenter
            anchors.horizontalCenter: flickablePinBoard.horizontalCenter
            width: flickablePinBoard.width-2
            height: pinBoard.height-2
            clip: true
            color: "#4d4d63"
            radius: 10

            Text{
                text: "Pinned buttons can be seen here."
                horizontalAlignment: Text.AlignHCenter
                color: "#FFFFFF"
                font.pointSize: 10
                anchors.centerIn: parent
                wrapMode: Text.Wrap


            }
        }
    }


    function refresh() {
        createPinButtons()
        createModuleButtons()
        if (!registerPlaceHolder.visible) {
            createRegisterButtons(backend.returnGlobalModuleId())
            if (!fieldPlaceHolder.visible) {
                createFieldButtons(backend.returnGlobalRegId())
                if (!confPlaceHolder.visible) {
                    createConfScreen(backend.returnGlobalFieldId())
                }
            }
        }
    }

    function createConfScreen(fieldId) {
        clearConf()
        var confType = backend.getConfType(fieldId);
        var isReadable = backend.getReadable(fieldId);
        var isWriteable = backend.getWriteable(fieldId);
        var resetValue = backend.getResetValue(fieldId);
        var currentValue = backend.sshGet(backend.getFieldAddr());

        var itemName
        var values

        switch (confType){
        case -1:
            itemName = "../components/Scoc3/confR.qml"
            break;
        case 0:
            if (isReadable) {
                itemName = "../components/Scoc3/confRWCombo.qml"
                values = backend.getValueDescriptions(fieldId)
            }
            else {
                itemName = "../components/Scoc3/confWCombo.qml"
                values = backend.getValueDescriptions(fieldId)
            }
            break;
        case 1:
            if (isReadable) {
                itemName = "../components/Scoc3/confRWText.qml"
            }
            else {
                itemName = "../components/Scoc3/confWText.qml"
            }
            break;
        }

        var moduleItem = Qt.createComponent(itemName)
        .createObject(confColumn, {
                          "Layout.alignment": Qt.AlignHCenter | Qt.AlignVCenter,
                          "valueList": values,
                          "resetValue": resetValue,
                          "currentValue": currentValue
                      });
        confPlaceHolder.visible = false
    }

//confscreen
    function createModuleButtons() {
        clearModules()
        backend.checkAllConfigValues(-1)
        var fileList = backend.getFileList()
        for(var i = 0; i < fileList.length; i++) {
            var name = fileList[i].split(".")[0]
            var moduleItem = Qt.createComponent("../components/Scoc3/module.qml")
            .createObject(moduleColumn, {
                              "moduleId": i,
                              "text": name,
                              "Layout.alignment": Qt.AlignHCenter | Qt.AlignVCenter,
                              "alert": backend.checkAllConfigValues(0, name),
                          });
            moduleItem.moduleClicked.connect(moduleButtonClicked)
        }
    }

    function moduleButtonClicked(moduleId) {
        backend.setGlobalRegId(-1)
        clearFields()
        clearConf()
        createRegisterButtons(moduleId)
    }
    //MODULE(FILE) BUTTONS END

    //REGISTER BUTTONS START
    function createRegisterButtons(moduleId) {
        clearRegisters()
        backend.checkAllConfigValues(-1, "")
        backend.setFilePath(moduleId)
        for(var i = 0; i < backend.getRegisterList().length; i++) {
            var name = backend.getRegisterList()[i]
            var registerItem = Qt.createComponent("../components/Scoc3/register.qml")
            .createObject(registerColumn, {
                              "registerId": i,
                              "text": name,
                              "Layout.alignment": Qt.AlignHCenter | Qt.AlignVCenter,
                              "alert": backend.checkAllConfigValues(1, (backend.getFileList()[backend.returnGlobalModuleId()].split(".")[0]+"."+name)),
                          });
            registerItem.registerClicked.connect(registerButtonClicked)
        }
        registerPlaceHolder.visible = false
    }

    function registerButtonClicked(registerId) {
        backend.setGlobalFieldId(-1)
        clearConf()
        createFieldButtons(registerId)
        createRegisterTabAlias(registerId)
    }

    function createRegisterTabAlias(registerId) {
        var name = backend.getRegisterList()[registerId]

        var duplicateAlert = false

        for (var i=0; i<registerTabRow.children.length; i++) {
            if ((name === registerTabRow.buttonNameList[i]) && (backend.returnGlobalModuleId() === registerTabRow.buttonModuleList[i])){duplicateAlert = true;}
        }

        if (!duplicateAlert) {
            var registerItem = Qt.createComponent("../components/Scoc3/registerTab.qml")
            .createObject(registerTabRow, {
                              "registerId": registerId,
                              "moduleId": backend.returnGlobalModuleId(),
                              "text": name,
                              "Layout.alignment": Qt.AlignHCenter | Qt.AlignVCenter,
                              "alert": backend.checkAllConfigValues(1, (backend.getFileList()[backend.returnGlobalModuleId()].split(".")[0]+"."+name)),
                          });
            registerItem.registerClicked.connect(registerButtonClicked)
            registerTabRow.buttonNameList.push(name)
            registerTabRow.buttonModuleList.push(backend.returnGlobalModuleId())
        }
    }

    function destroyRegisterTabAlias(registerId, moduleId) {
        var name = backend.getRegisterList()[registerId]
        console.log("PROCEEDING TO DESTROY", name)

        for (var i=0; i<registerTabRow.children.length; i++) {
            if ((name === registerTabRow.buttonNameList[i]) && (true)) {
                //moduleId === registerTabRow.buttonModuleList[i] instead of true ^
                registerTabRow.children[i].destroy()
                console.log("FLAG")
            }
        }


    }

    //REGISTER BUTTONS END

    //FIELD BUTTONS END
    function createFieldButtons(registerId) {
        clearFields()
        backend.checkAllConfigValues(-1, "")
        for(var i = 0; i < backend.getFieldList(registerId).length; i++) {
            var name = backend.getFieldList(registerId)[i]
            var fieldItem = Qt.createComponent("../components/Scoc3/field.qml")
            .createObject(fieldColumn, {
                              "fieldId": i,
                              "text": name,
                              "Layout.alignment": Qt.AlignHCenter | Qt.AlignVCenter,
                              "alert": backend.checkAllConfigValues(2, (backend.getFileList()[backend.returnGlobalModuleId()].split(".")[0]+"."+backend.getRegisterList()[backend.returnGlobalRegId()]+"."+name)),
                          });
            fieldItem.fieldClicked.connect(fieldButtonClicked)
        }
        fieldPlaceHolder.visible = false
    }

    function fieldButtonClicked(fieldId) {
        clearConf()
        createConfScreen(fieldId)
    }
    //FIELD BUTTONS END

    //PIN BUTTONS START
    function createPinButtons() {
        clearPinBoard()
        var pinButtonCount = backend.returnPinConfig("init")
        if (pinButtonCount === 0) {
            pinBoardPlaceHolder.visible = true
        }
        else {
            pinBoardPlaceHolder.visible = false
        }

        for (var i=0; i<pinButtonCount; i++) {
            var pinConfig = backend.returnPinConfig(i)
            var pinType = parseInt(pinConfig[0])


            if (i%2 == 0){
                switch (pinType) {
                    case 1:
                        var pinItem00 = Qt.createComponent("../components/Scoc3/pinButton.qml")
                        .createObject(pinButtonRow0, {
                                        "pinId": i,
                                        "text": pinConfig[pinType],
                                        "type": pinType,
                                        "module": pinConfig[1]
                                      });
                        break;
                    case 2:
                        var pinItem01 = Qt.createComponent("../components/Scoc3/pinButton.qml")
                        .createObject(pinButtonRow0, {
                                        "pinId": i,
                                        "text": pinConfig[pinType],
                                        "type": pinType,
                                        "module": pinConfig[1],
                                        "reg": pinConfig[2]
                                      });
                        break;
                    case 3:
                        var pinItem02 = Qt.createComponent("../components/Scoc3/pinButton.qml")
                        .createObject(pinButtonRow0, {
                                        "pinId": i,
                                        "text": pinConfig[pinType],
                                        "type": pinType,
                                        "module": pinConfig[1],
                                        "reg": pinConfig[2],
                                        "field": pinConfig[3]
                                      });
                        break;
                }

            }
            else {
                switch (pinType) {
                    case 1:
                        var pinItem10 = Qt.createComponent("../components/Scoc3/pinButton.qml")
                        .createObject(pinButtonRow1, {
                                        "pinId": i,
                                        "text": pinConfig[pinType],
                                        "type": pinType,
                                        "module": pinConfig[1]
                                      });
                        break;
                    case 2:
                        var pinItem11 = Qt.createComponent("../components/Scoc3/pinButton.qml")
                        .createObject(pinButtonRow1, {
                                        "pinId": i,
                                        "text": pinConfig[pinType],
                                        "type": pinType,
                                        "module": pinConfig[1],
                                        "reg": pinConfig[2]
                                      });
                        break;
                    case 3:
                        var pinItem12 = Qt.createComponent("../components/Scoc3/pinButton.qml")
                        .createObject(pinButtonRow1, {
                                        "pinId": i,
                                        "text": pinConfig[pinType],
                                        "type": pinType,
                                        "module": pinConfig[1],
                                        "reg": pinConfig[2],
                                        "field": pinConfig[3]
                                      });
                        break;
                }
            }


        }

    }

    //PIN BUTTONS END

    function clearModules() {
        for (var i = 0 ; i < moduleColumn.children.length; i++) {
            moduleColumn.children[i].destroy()
        }
    }

    function clearRegisters() {
        registerPlaceHolder.visible = true
        for (var i = 0 ; i < registerColumn.children.length; i++) {
            registerColumn.children[i].destroy()
        }
    }

    function clearFields() {
        fieldPlaceHolder.visible = true
        for (var i = 0 ; i < fieldColumn.children.length; i++) {
            fieldColumn.children[i].destroy()
        }
    }

    function clearConf() {
        confPlaceHolder.visible = true
        backend.resetConfigId()
        for (var i = 0 ; i < confColumn.children.length; i++) {
            confColumn.children[i].destroy()
        }
    }

    function clearPinBoard(){
        for (var i = 0 ; i < pinButtonRow0.children.length; i++) {
            pinButtonRow0.children[i].destroy()
        }
        for (var i = 0 ; i < pinButtonRow1.children.length; i++) {
            pinButtonRow1.children[i].destroy()
        }
    }

    Connections { target: backend }
}
