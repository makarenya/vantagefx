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

  ListView {
    id: view
    Layout.fillHeight: true
    anchors.left: parent.left
    anchors.right: parent.right
    spacing: 10
    model: root.options
    anchors {
      fill: parent
      leftMargin: 10
      topMargin: 46
      rightMargin: 10
      bottomMargin: 10
    }

    delegate: OptionLine {
      anchors.left: parent.left
      anchors.right: parent.right
      height: 20
    }
  }
}
