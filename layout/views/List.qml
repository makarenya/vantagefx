import QtQuick 2.5
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.2

ColumnLayout {
  anchors {
    fill: parent
    margins: 10
  }
  spacing: 10

  LoginArea {
    anchors.left: parent.left
    anchors.right: parent.right
    height: 30
  }

  Rectangle {
    anchors.left: parent.left
    anchors.right: parent.right
    anchors.leftMargin: 10
    anchors.rightMargin: 10
    Layout.preferredHeight: 30

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
      width: 80
      height: 22
      text: "Update bet"
      onClicked: root.setBet(parseInt(firstBet.text), parseInt(betGrowth.text));
    }

    Text {
      x: 390
      y: 4
      width: 100
      text: "Current bet: " + root.currentBet
      renderType: Text.NativeRendering
    }
  }

  Rectangle {
    Layout.fillHeight: true
    anchors.left: parent.left
    anchors.right: parent.right

    ListView {
      id: view
      spacing: 10
      anchors.fill: parent
      anchors.leftMargin: 10
      anchors.rightMargin: 10
      model: root.options

      delegate: OptionLine {
        anchors.left: parent.left
        anchors.right: parent.right
        height: 20
      }
    }
  }
}
