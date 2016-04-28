import QtQuick 2.5
import QtQuick.Window 2.2
import QtQuick.Controls 1.4

Window {
    visible: true
    width: 360
    height: 520

    Rectangle {
        anchors.fill: parent
        visible: !root.loaded
        Text {
            anchors.centerIn: parent
            text: "Loading..."
            font.pixelSize: 24
            renderType: Text.NativeRendering
        }
    }

    Rectangle {
        anchors.fill: parent
        visible: root.loaded
        ListView {
            id: view
            anchors.fill: parent
            anchors.margins: 10
            spacing: 10
            model: root.options

            delegate: Item {
                width: view.width
                height: 20

                Rectangle {
                    id: item
                    anchors.fill: parent

                    Text {
                        x: 10
                        y: 3
                        width: 120
                        height: 16

                        renderType: Text.NativeRendering
                        text: model.name
                    }

                    Text {
                        x: 130
                        y: 3
                        width: 20
                        height: 16
                        text: model.low
                        renderType: Text.NativeRendering
                    }

                    Text {
                        x: 270
                        y: 3
                        width: 20
                        height: 16
                        text: model.high
                        renderType: Text.NativeRendering
                    }

                    Rectangle {
                        x: 160
                        y: 3
                        width: model.low
                        height: 16
                        color: "#ff0000"
                    }

                    Rectangle {
                        x: 160 + model.low
                        y: 3
                        width: model.high
                        height: 16
                        color: "#00ff00"
                    }
                }
            }
        }
    }
}