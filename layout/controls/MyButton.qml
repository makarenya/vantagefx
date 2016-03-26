import QtQuick 2.5

Item {
    id: button
    width: 200
    height: 20
    signal click

    Rectangle {
        id: buttonLayout
        width: button.width
        height: button.height
        color: mouseArea.containsPress ? "#cce4f7" : mouseArea.containsMouse ? "#e5f1fb" : "#e1e1e1"
        border.color: mouseArea.containsPress ? "#005499" : mouseArea.containsMouse ? "#0078d7" : "#adadad"
        smooth: true

        Text {
            id: text1
            x: 58
            y: 9
            text: qsTr("Clear text")
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenter: parent.verticalCenter
            font.family: "Segoe UI"
            font.pixelSize: 12
            renderType: Text.NativeRendering
        }

        MouseArea {
            id: mouseArea
            anchors.fill: parent
            hoverEnabled: true
            onClicked: click()
        }
    }
}