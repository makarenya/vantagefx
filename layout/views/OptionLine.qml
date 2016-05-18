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

    Rectangle {
      Layout.preferredWidth: 70
      height: 20

      Button {
        width: 50
        height: 20
        visible: model.option_30 > 0
        text: "30"
        onClicked: root.options.select(model.option_30)
      }

      Rectangle {
        anchors.right: parent.right
        y: 3
        width: 14
        height: 14
        border {
          width: 1
          color: "#000000"
        }
        visible: model.option_30 > 0
        color: model.color_30
      }
    }

    Rectangle {
      Layout.preferredWidth: 70
      height: 20

      Button {
        width: 50
        height: 20
        visible: model.option_60 > 0
        text: "60"
        onClicked: root.options.select(model.option_60)
      }

      Rectangle {
        anchors.right: parent.right
        y: 3
        width: 14
        height: 14
        border {
          width: 1
          color: "#000000"
        }
        visible: model.option_60 > 0
        color: model.color_60
      }
    }

    Rectangle {
      Layout.preferredWidth: 70
      height: 20

      Button {
        width: 50
        height: 20
        visible: model.option_120 > 0
        text: "2m"
        onClicked: root.options.select(model.option_120)
      }

      Rectangle {
        anchors.right: parent.right
        y: 3
        width: 14
        height: 14
        border {
          width: 1
          color: "#000000"
        }
        visible: model.option_120 > 0
        color: model.color_120
      }
    }

    Rectangle {
      Layout.preferredWidth: 70
      height: 20

      Button {
        width: 50
        height: 20
        visible: model.option_300 > 0
        text: "5m"
        onClicked: root.options.select(model.option_300)
      }

      Rectangle {
        anchors.right: parent.right
        y: 3
        width: 14
        height: 14
        border {
          width: 1
          color: "#000000"
        }
        visible: model.option_300 > 0
        color: model.color_300
      }
    }

    Rectangle {
      Layout.fillWidth: true
    }
  }
}