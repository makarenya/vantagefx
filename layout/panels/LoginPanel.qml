import QtQuick 2.5
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.2

Rectangle {

  Rectangle {
    visible: !root.loggedIn
    anchors.fill: parent
    anchors.leftMargin: 10
    anchors.rightMargin: 10

    Button {
      text: "Login"
      onClicked: root.doLogin()
    }
  }

  Rectangle {
    visible: root.loggedIn
    anchors.fill: parent
    anchors.leftMargin: 10
    anchors.rightMargin: 10

    Text {
      text: root.fullName
      renderType: Text.NativeRendering
    }

    Text {
      anchors.right: parent.right
      text: root.money
      renderType: Text.NativeRendering
    }
  }
}