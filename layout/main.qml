import QtQuick 2.5
import QtQuick.Window 2.2
import QtQuick.Controls 1.4

Window {
  property int itemAngle: 60
  property int itemSize: 300

  visible: true
  width: 360
  height: 360

  Rectangle {
    anchors.fill: parent
ListModel {
        id: dataModel

        ListElement {
            color: "orange"
            text: "first"
        }
        ListElement {
            color: "lightgreen"
            text: "second"
        }
        ListElement {
            color: "orchid"
            text: "third"
        }
        ListElement {
            color: "tomato"
            text: "fourth"
        }
    }

    Flickable {
        anchors.fill: parent
        contentWidth: row.width
        flickDeceleration: 1000

        Row {
            id: row

            height: parent.height

            Repeater {
                model: dataModel
                delegate: Item {
                    height: parent.height
                    width: 100

                    Rectangle {
                        anchors.margins: 5
                        anchors.fill: parent
                        color: model.color
                        border {
                            color: "black"
                            width: 1
                        }

                        Text {
                            anchors.centerIn: parent
                            renderType: Text.NativeRendering
                            text: model.text
                        }
                    }
                }
            }
        }
    }
 }
}