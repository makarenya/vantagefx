import QtQuick 2.5
import QtQuick.Window 2.2
import QtQuick.Controls 1.4
import "views"

Window {
    visible: true
    width: 760
    height: 520

    Login {
        anchors.fill: parent
        visible: root.mode == "login"
    }

    Loading {
        anchors.fill: parent
        visible: root.mode == "" || root.mode == "purchasing"
    }

    List {
        anchors.fill: parent
        visible: root.mode == "view"
    }

    Details {
        anchors.centerIn: parent
        visible: root.mode == "details"
    }
}