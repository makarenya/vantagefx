import QtQuick 2.5
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.2

Rectangle {

  RowLayout {
    anchors.fill: parent
    spacing: 10

    Text {
      Layout.preferredWidth: 120
      renderType: Text.NativeRendering
      text: model.name
    }

    Text {
      Layout.preferredWidth: 40
      text: model.ratePut + " / " + model.rateOut
      renderType: Text.NativeRendering
    }

    Rectangle {
      Layout.preferredWidth: 100
      height: 20

      Rectangle {
        x: 29
        y: 0
        width: 1
        height: 20
        color: "black"
      }

      Rectangle {
        x: 69
        y: 0
        width: 1
        height: 20
        color: "black"
      }

      Rectangle {
        y: 1
        width: model.ratePut
        height: 8
        color: "#ff0000"
      }

      Rectangle {
        y: 1
        x: model.ratePut
        width: model.rateCall
        height: 8
        color: "#00ff00"
      }

      Rectangle {
        y: 11
        width: model.rateOut
        height: 8
        color: "#ff0000"
      }

      Rectangle {
        y: 11
        x: model.rateOut
        width: model.rateIn
        height: 8
        color: "#00ff00"
      }
    }

    Text {
      Layout.preferredWidth: 40
      text: model.rateCall + " / " + model.rateIn
      renderType: Text.NativeRendering
    }

    Repeater {
      model: options

      delegate: Rectangle {
        Layout.preferredWidth: 76
        height: 24

        Button {
          width: 50
          height: 24
          text: model.name
          onClicked: model.selected = !model.selected
        }

        Rectangle {
          anchors.right: parent.right
          y: 3
          width: 18
          height: 18
          border.width: 1
          border.color: model.border
          color: model.background
        }
      }
    }

    Rectangle {
      Layout.fillWidth: true
    }

    Rectangle {
      Layout.preferredWidth: 80
      height: 24

      Button {
        width: 80
        height: 24
        text: "Remove"
        onClicked: root.stopWatch(model.asset)
      }
    }
  }
}