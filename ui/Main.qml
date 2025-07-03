import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import App.Context 1.0

ApplicationWindow {
    id: root
    width: 1280
    height: 720
    visible: true
    title: "AvaSYS GCS 2.0"

    background: Rectangle { color: "#1e1e1e" }

    header: ToolBar {
        Label {
            text: "AvaSYS Ground Control"
            font.bold: true
            color: "white"
            padding: 10
        }
    }

    Item {
        anchors.fill: parent
        MainPanel {
            anchors.fill: parent
        }
        BottomPanel {
            id: bottomPanel1
            anchors {
                left: parent.left
                right: parent.right
                bottom: parent.bottom
            }
        }
    }
}