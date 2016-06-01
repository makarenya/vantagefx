import QtQuick 2.5
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.2

Rectangle {
  Text {
    y: 4
    width: 80
    text: "First bet"
    renderType: Text.NativeRendering
    horizontalAlignment: Text.AlignRight
  }

  TextField {
    id: firstBet
    x: 90
    width: 40
    height: 22
    text: root.firstBet
  }

  Text {
    x: 140
    y: 4
    width: 80
    text: "Bet growth"
    renderType: Text.NativeRendering
    horizontalAlignment: Text.AlignRight
  }

  TextField {
    id: betGrowth
    x: 230
    width: 40
    height: 22
    text: root.betGrowth
  }

  Button {
    x: 300
    width: 100
    height: 22
    text: "Update bet"
    onClicked: root.setBet(parseInt(firstBet.text), parseInt(betGrowth.text));
  }
}
