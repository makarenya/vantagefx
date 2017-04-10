import QtQuick 2.5
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.2

Rectangle {
  Text {
    y: 4
    width: 80
    text: "Bet amount"
    renderType: Text.NativeRendering
    horizontalAlignment: Text.AlignRight
  }

  TextField {
    id: betAmount
    x: 90
    width: 40
    height: 22
    text: root.betAmount
  }

  Text {
    x: 140
    y: 4
    width: 80
    text: "Bets count"
    renderType: Text.NativeRendering
    horizontalAlignment: Text.AlignRight
  }

  TextField {
    id: betsCount
    x: 230
    width: 40
    height: 22
    text: root.betsCount
  }

  Button {
    x: 300
    width: 100
    height: 22
    text: "Update bet"
    onClicked: root.setBet(parseInt(betAmount.text), parseInt(betsCount.text));
  }
}
