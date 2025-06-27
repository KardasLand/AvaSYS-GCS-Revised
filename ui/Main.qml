// file: ui/Main.qml
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

    // Main layout with map and side panels
    RowLayout {
        anchors.fill: parent

        // Main Map and Info Display
        RightPanel {
            Layout.fillWidth: true
            Layout.fillHeight: true
        }
    }

    // A drawer for connection settings
    // ConnectionDrawer {
    //     id: connectionDrawer
    // }

    // // Bottom telemetry bar
    // BottomPanel {
    //     anchors.left: parent.left
    //     anchors.right: parent.right
    //     anchors.bottom: parent.bottom
    // }

    // Button to open the connection drawer
    // Button {
    //     anchors.top: parent.top
    //     anchors.right: parent.right
    //     anchors.margins: 10
    //     text: "Connect"
    //     onClicked: connectionDrawer.open()
    // }
}