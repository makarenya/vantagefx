import QtQuick 2.5
import QtQuick.Window 2.2
import QtQuick.Controls 1.4
import "views"

Window {
    visible: true
    width: 600
    height: 520

    Login {
        anchors.fill: parent
        visible: root.mode == "login"
    }

    Loading {
        anchors.fill: parent
        visible: root.mode == ""
    }

    List {
        anchors.fill: parent
        visible: root.mode == "view"
    }

    Details {
        anchors.fill: parent
        visible: root.mode == "details"
    }
}