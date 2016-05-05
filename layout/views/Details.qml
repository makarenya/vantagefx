import QtQuick 2.5
import QtQuick.Controls 1.4

Rectangle {
  width: 140
  height: 200
  Text {
    x: 10
    y: 10
    text: root.optionName
  }

  Text {
    x: 10
    y: 40
    text: (root.optionReturn - 100) + "%"
  }

  Text {
    x: 10
    y: 70
    text: root.optionExpire
  }

  Button {
    x: 10
    y: 100
    width: 120
    text: "Close"
    onClicked: root.cancelLogin()
  }
}
