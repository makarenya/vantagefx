import QtQuick 2.5
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.2
import "../panels"

ColumnLayout {
  anchors {
    fill: parent
    margins: 10
  }
  spacing: 10

  LoginPanel {
    anchors.left: parent.left
    anchors.right: parent.right
    height: 30
  }

  BetPanel {
    anchors.left: parent.left
    anchors.right: parent.right
    anchors.leftMargin: 10
    anchors.rightMargin: 10
    Layout.preferredHeight: 30
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
      anchors.bottomMargin: 40
      model: root.options

      delegate: OptionLine {
        anchors.left: parent.left
        anchors.right: parent.right
        height: 24
      }
    }
    ComboBox {
      anchors.left: parent.left
      anchors.bottom: parent.bottom
      anchors.leftMargin: 10
      anchors.bottomMargin: 1
      width: 200
      id: addBox
      model: root.assets
    }
    Button {
      anchors.left: parent.left
      anchors.bottom: parent.bottom
      anchors.leftMargin: 220
      width: 100
      text: "Add item"
      onClicked: root.watch(addBox.currentIndex)
    }
  }
}
