import QtQuick 2.5
import QtQuick.Controls 1.4

Rectangle {
    Text {
        anchors.centerIn: parent
        text: root.description
        font.pixelSize: 24
        renderType: Text.NativeRendering
    }
}
