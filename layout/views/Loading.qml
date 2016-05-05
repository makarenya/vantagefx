import QtQuick 2.5
import QtQuick.Controls 1.4

Rectangle {
    Text {
        anchors.centerIn: parent
        text: "Loading..."
        font.pixelSize: 24
        renderType: Text.NativeRendering
    }

    Text {
        anchors.centerIn: parent
        anchors.verticalCenterOffset: 30
        font.pixelSize: 12
        text: root.description
        renderType: Text.NativeRendering
    }
}
