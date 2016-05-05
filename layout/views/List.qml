import QtQuick 2.5
import QtQuick.Controls 1.4

Rectangle {

    Rectangle {
        x: 0
        y: 0
        width: parent.width
        height: 45
        visible: root.fullName == ""
        Button {
            x: 10
            y: 10
            text: "Login"
            onClicked: root.doLogin()
        }
    }
    Rectangle {
        x: 0
        y: 0
        width: parent.width
        height: 45
        visible: root.fullName != ""
        Text {
            x: 20
            y: 10
            text: root.fullName
            renderType: Text.NativeRendering
        }
        Text {
            x: 200
            y: 10
            text: root.money
            renderType: Text.NativeRendering
        }
    }
    ListView {
        id: view
        anchors {
            fill: parent
            leftMargin: 10
            topMargin: 46
            rightMargin: 10
            bottomMargin: 10
        }
        spacing: 10
        model: root.options

        delegate: Item {
            height: 20
            width: view.width

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
                    x: 150
                    y: 3
                    width: 20
                    height: 16
                    text: model.low
                    renderType: Text.NativeRendering
                }

                Text {
                    x: 290
                    y: 3
                    width: 20
                    height: 16
                    text: model.high
                    renderType: Text.NativeRendering
                }

                Rectangle {
                    x: 180
                    y: 3
                    width: model.low
                    height: 16
                    color: "#00ff00"
                }

                Rectangle {
                    x: 180 + model.low
                    y: 3
                    width: model.high
                    height: 16
                    color: "#ff0000"
                }

                Button {
                    x: 320
                    y: 0
                    width: 50
                    height: 22
                    visible: model.option_30 > 0
                    text: "30"
                    onClicked: root.view(model.option_30)
                }

                Button {
                    x: 380
                    y: 0
                    width: 50
                    height: 22
                    visible: model.option_60 > 0
                    text: "60"
                    onClicked: root.view(model.option_60)
                }

                Button {
                    x: 440
                    y: 0
                    width: 50
                    height: 22
                    visible: model.option_120 > 0
                    text: "2m"
                    onClicked: root.view(model.option_120)
                }

                Button {
                    x: 500
                    y: 0
                    width: 50
                    height: 22
                    visible: model.option_300 > 0
                    text: "5m"
                    onClicked: root.view(model.option_300)
                }
            }
        }
    }
}
