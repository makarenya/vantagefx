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
      Layout.preferredWidth: 20
      text: model.high
      renderType: Text.NativeRendering
    }

    Rectangle {
      Layout.preferredWidth: 100
      height: 16

      Rectangle {
        width: model.low
        height: 16
        color: "#00ff00"
      }

      Rectangle {
        x: model.low
        width: model.high
        height: 16
        color: "#ff0000"
      }
    }

    Text {
      Layout.preferredWidth: 20
      text: model.low
      renderType: Text.NativeRendering
    }

    TextField {
      id: threshold
      Layout.preferredWidth: 50
      height: 22
      text: model.threshold
    }

    Binding { target: model; property: "threshold"; value: threshold.text }

    Repeater {
      model: options

      delegate: Rectangle {
        Layout.preferredWidth: 90
        height: 20

        Button {
          width: 50
          height: 20
          text: model.name
          onClicked: model.selected = !model.selected
        }

        Rectangle {
          anchors.right: parent.right
          y: 1
          width: 30
          height: 18
          border.width: 1
          border.color: model.border
          color: model.background

          Text {
            width: 30
            height: 18
            text: model.bet
            renderType: Text.NativeRendering
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            color: model.foreground
          }
        }
      }
    }

    Rectangle {
      Layout.fillWidth: true
    }

    Rectangle {
      Layout.preferredWidth: 80
      height: 20

      Button {
        width: 80
        height: 20
        text: "Remove"
        onClicked: root.stopWatch(model.asset)
      }
    }
  }
}