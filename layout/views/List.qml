import QtQuick 2.5
import QtQuick.Controls 1.4

Rectangle {

    Rectangle {
        x: 0
        y: 0
        width: parent.width
        height: 45
        visible: !root.loggedIn
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
        visible: root.loggedIn
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

                TextField {
                    id: threshold
                    x: 320
                    y: 0
                    width: 50
                    height: 22
                    text: model.threshold
                }
                Binding { target: model; property: "threshold"; value: threshold.text }

                Button {
                    x: 380
                    y: 0
                    width: 50
                    height: 22
                    visible: model.option_30 > 0
                    text: "30"
                    checkable: true
                    onClicked: root.selectOption(model.option_30, 30, checked)
                }

                Button {
                    x: 440
                    y: 0
                    width: 50
                    height: 22
                    visible: model.option_60 > 0
                    text: "60"
                    checkable: true
                    onClicked: root.selectOption(model.option_60, 60, checked)
                }

                Button {
                    x: 500
                    y: 0
                    width: 50
                    height: 22
                    visible: model.option_120 > 0
                    text: "2m"
                    checkable: true
                    onClicked: root.selectOption(model.option_120, 120, checked)
                }

                Button {
                    x: 560
                    y: 0
                    width: 50
                    height: 22
                    visible: model.option_300 > 0
                    text: "5m"
                    checkable: true
                    onClicked: root.selectOption(model.option_300, 300, checked)
                }
            }
        }
    }
}
