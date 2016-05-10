import QtQuick 2.5
import QtQuick.Controls 1.4

Rectangle {
  width: 140
  height: 220
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

  Button {
    x: 30
    y: 150
    width: 80
    text: "High"
    onClicked: root.buyHigh()
    visible: root.loggedIn
  }

  Button {
    x: 30
    y: 180
    width: 80
    text: "Low"
    onClicked: root.buyLow()
    visible: root.loggedIn
  }
}
