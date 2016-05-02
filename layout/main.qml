import QtQuick 2.5
import QtQuick.Window 2.2
import QtQuick.Controls 1.4

Window {
    visible: true
    width: 600
    height: 520

    Rectangle {
        anchors.fill: parent
        visible: root.mode == "login"
        Rectangle {
            anchors.centerIn: parent
            width: 300
            height: 300
            Text {
                x: 10
                y: 10
                width: 80
                horizontalAlignment: Text.AlignRight
                text: "Login"
                renderType: Text.NativeRendering
            }
            Text {
                x: 10
                y: 53
                width: 80
                horizontalAlignment: Text.AlignRight
                text: "Password"
                renderType: Text.NativeRendering
            }
            Text {
                x: 10
                y: 93
                width: 80
                horizontalAlignment: Text.AlignRight
                text: "Server"
                renderType: Text.NativeRendering
            }
            TextField {
                x: 100
                y: 10
                width: 170
                id: login
                text: root.login
            }
            Binding { target: root; property: "login"; value: login.text }
            TextField {
                x: 100
                y: 50
                width: 170
                id: password
                text: root.password
            }
            Binding { target: root; property: "password"; value: password.text }
            ComboBox {
                x: 100
                y: 90
                width: 170
                id: server
                model: root.servers
                onCurrentTextChanged: {
                    root.server = server.currentText;
                }
            }
            Button {
                x: 50
                y: 130
                width: 100
                text: "Login"
                onClicked: root.processLogin()
            }
            Button {
                x: 150
                y: 130
                width: 100
                text: "Cancel"
                onClicked: root.cancelLogin()
            }
        }
    }

    Rectangle {
        anchors.fill: parent
        visible: root.mode == ""
        Text {
            anchors.centerIn: parent
            text: "Loading..."
            font.pixelSize: 24
            renderType: Text.NativeRendering
        }
    }

    Rectangle {
        anchors.fill: parent
        visible: root.mode == "view"
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
                x: 100
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

                    Text {
                        x: 320
                        y: 3
                        width: 50
                        height: 16
                        text: Number(model.price).toLocaleString(Qt.locale(), "f", 2)
                        renderType: Text.NativeRendering
                    }

                    Text {
                        x: 380
                        y: 3
                        width: 40
                        height: 16
                        text: (model.back - 100) + "%"
                        renderType: Text.NativeRendering
                    }

                    Rectangle {
                        x: 420
                        y: 3
                        height: 16
                        width: 30
                        color: "#ffff70"
                        visible: model.option_30 > 0
                        Text {
                            text: "30"
                            width: 30
                            renderType: Text.NativeRendering
                            horizontalAlignment: Text.AlignHCenter
                        }
                    }

                    Rectangle {
                        x: 460
                        y: 3
                        height: 16
                        width: 30
                        color: "#ffff70"
                        visible: model.option_60 > 0
                        Text {
                            text: "60"
                            width: 30
                            renderType: Text.NativeRendering
                            horizontalAlignment: Text.AlignHCenter
                        }
                    }

                    Rectangle {
                        x: 500
                        y: 3
                        height: 16
                        width: 30
                        color: "#ffff70"
                        visible: model.option_120 > 0
                        Text {
                            text: "2m"
                            width: 30
                            renderType: Text.NativeRendering
                            horizontalAlignment: Text.AlignHCenter
                        }
                    }

                    Rectangle {
                        x: 540
                        y: 3
                        height: 16
                        width: 30
                        color: "#ffff70"
                        visible: model.option_300 > 0
                        Text {
                            text: "5m"
                            width: 30
                            renderType: Text.NativeRendering
                            horizontalAlignment: Text.AlignHCenter
                        }
                    }
                }
            }
        }
    }
}