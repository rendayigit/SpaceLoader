import QtQuick 2.15
import QtQuick.Controls 2.15

Rectangle {
    id: rectangle

    property string connectPc: "Connected to Starkit"

    width: 350
    height: 50
    color: "#27273a"
    radius: 10
    border.width: 3

    Row {
        id: row

        width: row.width = rectangle.width
        height: row.height = rectangle.height
        leftPadding: 20
        bottomPadding: 10
        topPadding: 10
        spacing: 50

        Text {
            id: text1

            color: "#ffffff"
            text: connectPc
            font.pixelSize: 24
        }

        RoundButton {
            id: roundButton

            text: "X"
            font.bold: true
            font.pointSize: 10
            padding: 3
        }

    }

}
