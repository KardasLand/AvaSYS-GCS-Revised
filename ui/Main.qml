import QtQuick
import QtQuick.Controls.FluentWinUI3
import QtQuick.Layouts

import App.Context 1.0

ApplicationWindow {
    id: root
    width: 1280
    height: 720
    visible: true
    title: "AvaSYS GCS 2.0"

    background: Rectangle { color: "#1e1e1e" }
    property var vehicle: AppContext.vehicleManager.mainVehicle

    Timer {
        interval: 200; running: true; repeat: true
        onTriggered: {
            // This polling pattern is robust if the mainVehicle can appear/disappear
            if (root.vehicle !== AppContext.vehicleManager.mainVehicle) {
                root.vehicle = AppContext.vehicleManager.mainVehicle;
            }
        }
    }

    header: ToolBar {
        id: header
        Label {
            text: "AvaSYS GCS 2.0"
            color: "white"
            font.bold: true
            padding: 10
        }
        RowLayout {
            anchors.right: parent.right
            spacing: 10
            Rectangle {
                color: root.vehicle.isArmed ? "#4CAF50" : "#F44336"
                width: 20; height: 20; radius: 10;
                Layout.alignment: Qt.AlignVCenter
            }
            Label {
                text: root.vehicle.isArmed ? "Armed" : "Disarmed"
                color: "white"
                font.bold: true
                verticalAlignment: Text.AlignVCenter
            }
            Button {
                text: "Settings"
                onClicked: {
                    if (!child_window_comp.visible) {
                        var window = child_window_comp.createObject(root)
                        window.show()
                    }
                }
            }
            Button {
                text: "Exit"
                onClicked: Qt.quit()
            }
        }
    }

    Item {
        anchors.fill: parent
        MainPanel {
            id: mainPanel
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
        HorizonPanel {
            id: horizonPanel
        }
    }

    Component {
        id: child_window_comp
        SettingsWindow {

        }
    }
}