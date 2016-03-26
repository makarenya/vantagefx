import QtQuick 2.5

Item {
    id: textBox
    width: 200
    height: 25
    property alias text: textInput.text

    Rectangle {
        id: textLayout
        width: textBox.width
        height: textBox.height
        color: "#ffffff"
        radius: 0
        smooth: true
        border.color: "#828790"

        TextInput {
            id: textInput
            font.family: "Segoe UI"
            anchors.fill: parent
            anchors.margins: 4
            font.pixelSize: 14
            horizontalAlignment: TextInput.AlignLeft
            renderType: Text.NativeRendering
        }
    }
}