import QtQuick 2.5
import QtQuick.Controls 1.4

Rectangle {

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