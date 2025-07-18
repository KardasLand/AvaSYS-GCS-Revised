import QtQuick
import QtQuick.Layouts
import QtQuick.Controls.FluentWinUI3

import App.Context 1.0

Rectangle {
    id: bottomPanel
    height: 120
    color: "#212121"
    anchors.bottom: parent.bottom
    anchors.left: parent.left
    anchors.right: parent.right


    //todo refactor this shit

    property var vehicle: AppContext.vehicleManager.mainVehicle
    property var mavlinkManager: AppContext.mavlinkManager
    property bool isHssVisible: false
    signal toggleHss()

    // --- Mock Data Properties - Initialized to match the new image ---
    property real mockLatitude: 37.523051
    property real mockLongitude: -122.255749
    property real mockAltitude: 32.86
    property real mockSpeed: 16.8
    property real mockRoll: 15.7
    property real mockHeading: 130
    property real mockVoltage: 15.3
    property real mockCurrent: -0.8
    property real mockBattery: 100.9

    Popup {
        id: altitudePopup
        anchors.centerIn: Overlay.overlay

        modal: true
        focus: true
        dim: false

        background: Rectangle {
            color: "#2C2C2E"; radius: 12
            border.color: "#A0A0A0"; border.width: 1
        }
        ColumnLayout {
            spacing: 2
            anchors.fill: parent
            Text {
                text: "Enter Altitude for Takeoff"
                color: "white"
                font.pixelSize: 16
                font.bold: true
                horizontalAlignment: Text.AlignHCenter
            }

            TextField {
                id: altitudeInput
                placeholderText: "Altitude in meters"
                width: parent.width - 40
                font.pixelSize: 14
                color: "white"
                background: Rectangle { color: "#3C3C3E"; radius: 5 }
            }

            Button {
                text: "Confirm"
                onClicked: {
                    let altitude = parseFloat(altitudeInput.text);
                    if (!isNaN(altitude)) {
                        bottomPanel.mavlinkManager.sendTakeoffCommand(bottomPanel.vehicle.systemId, altitude);
                        altitudePopup.close();
                    } else {
                        console.warn("Invalid altitude input");
                    }
                }
            }
        }

        closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutsideParent
    }

    Timer {
        interval: 200; running: true; repeat: true
        onTriggered: {
            bottomPanel.vehicle = AppContext.vehicleManager.mainVehicle;
            bottomPanel.mavlinkManager = AppContext.mavlinkManager;
            if (!bottomPanel.vehicle) {
                // If no vehicle is available, use mock data
                mockSpeed = mockSpeed || 16.8;
                mockRoll = mockRoll || 15.7;
                mockHeading = mockHeading || 130;
                mockAltitude = mockAltitude || 32.86;
                mockVoltage = mockVoltage || 15.3;
                mockLatitude = mockLatitude || 37.523051;
                mockLongitude = mockLongitude || -122.255749;
                mockBattery = mockBattery || 100.9;
                mockCurrent = mockCurrent || -0.8;
            } else {
                // Update with real vehicle data
                mockSpeed = bottomPanel.vehicle.groundSpeed;
                mockRoll = bottomPanel.vehicle.roll;
                if (mockRoll > 45) mockRoll = 45; if (mockRoll < -45) mockRoll = -45;
                mockHeading = bottomPanel.vehicle.heading;
                mockAltitude = bottomPanel.vehicle.altitude;
                mockVoltage = bottomPanel.vehicle.batteryVoltage;
                mockLatitude = bottomPanel.vehicle.coordinate.latitude;
                mockLongitude = bottomPanel.vehicle.coordinate.longitude;
                mockBattery = bottomPanel.vehicle.batteryRemaining
                mockCurrent = bottomPanel.vehicle.batteryCurrent;
            }
        }
    }

    RowLayout {
        anchors.fill: parent
        anchors.leftMargin: 30; anchors.rightMargin: 30

        ColumnLayout {
            Layout.alignment: Qt.AlignVCenter; spacing: 5
            Component { id: dataRow; Row { spacing: 8; Label { text: title + ":"; color: "#A0A0A0"; font.pixelSize: 14; width: 75; horizontalAlignment: Text.AlignRight } Label { text: value; color: "white"; font.pixelSize: 14; font.weight: Font.Bold } } }
            Loader { sourceComponent: dataRow; property string title: "Latitude"; property string value: mockLatitude.toFixed(6) + "°" }
            Loader { sourceComponent: dataRow; property string title: "Longitude"; property string value: mockLongitude.toFixed(6) + "°" }
            Loader { sourceComponent: dataRow; property string title: "Altitude"; property string value: mockAltitude.toFixed(2) + " m" }
        }
        Item { Layout.fillWidth: true }

        ColumnLayout {
            // series of buttons to control the vehicle, toggle the visibility of components etc..
            Layout.alignment: Qt.AlignVCenter; spacing: 5
            Button {
                text: vehicle.isFlying ? "Land" : "Takeoff"
                background: Rectangle {
                    color: vehicle.isFlying ? "#F44336" : "#4CAF50"
                    radius: 5
                }
                Layout.preferredWidth: 150
                onClicked: {
                    if (vehicle.isFlying) {
                        console.log("Land button clicked", vehicle.vehicleId, vehicle.systemId);
                        mavlinkManager.sendLandCommand(vehicle.systemId);
                    } else {
                        console.log("Takeoff button clicked");
                        if (altitudePopup.visible) {
                            altitudePopup.close();
                        } else {
                            altitudePopup.open();
                        }
                    }
                }
            }
            Button {
                text: vehicle.isArmed ? "Disarm" : "Arm (Shift for Force Arm)"
                background: Rectangle {
                    color: !vehicle.isArmed ? "#F44336" : "#4CAF50"
                    radius: 5
                }
                Layout.preferredWidth: 150
                MouseArea {
                    anchors.fill: parent
                    onClicked: (mouse) => {
                        let isArmed = vehicle.isArmed;
                        let force = mouse.modifiers & Qt.ShiftModifier;

                        console.log("Arm/Disarm button clicked",
                            "Vehicle ID:", vehicle.vehicleId,
                            "System ID:", vehicle.systemId,
                            "Current State:", isArmed,
                            "Force Arm:", force);

                        mavlinkManager.sendArmCommand(vehicle.systemId, !isArmed, force);
                    }
                }
            }
            Button {
                text: "Missions"
                Layout.preferredWidth: 150
                onClicked: {
                    console.log("Takeoff button clicked");
                }
            }
        }
        Item { Layout.fillWidth: true }

        Rectangle {
            color: "#2C2C2E"; radius: 12
            Layout.preferredWidth: 450; Layout.fillHeight: true
            Layout.topMargin: 10; Layout.bottomMargin: 10

            ColumnLayout {
                anchors.fill: parent
                spacing: 0

                Item {
                    Layout.fillHeight: true
                }

                RowLayout {
                    Layout.alignment: Qt.AlignHCenter
                    Layout.bottomMargin: 5
                    spacing: 15

                    StyledGauge {
                        Layout.preferredWidth: 120; Layout.preferredHeight: 120
                        title: "Speed"; unit: "m/s"
                        value: mockSpeed; from: 0; to: 30; decimals: 1; majorTickCount: 6
                    }
                    StyledGauge {
                        Layout.preferredWidth: 120; Layout.preferredHeight: 120
                        title: "Roll"; unit: "deg"
                        value: mockRoll; from: -45; to: 45; decimals: 1; majorTickCount: 6
                    }
                    CompassGauge {
                        Layout.preferredWidth: 120; Layout.preferredHeight: 120
                        value: mockHeading
                    }
                }
            }
        }

        Item { Layout.fillWidth: true }
        ColumnLayout {
            // series of buttons to control the vehicle, toggle the visibility of components etc..
            Layout.alignment: Qt.AlignVCenter; spacing: 5
            Button {
                text: "Toggle PFD"
                Layout.preferredWidth: 150
                background: Rectangle {
                    color: horizonPanel.visible ? "#4CAF50" : "#F44336"
                    radius: 5
                }
                onClicked: {
                    horizonPanel.visible = !horizonPanel.visible;
                }
            }
            Button {
                text: "Toggle HSS"
                Layout.preferredWidth: 150
                background: Rectangle {
                    color: bottomPanel.isHssVisible ? "#4CAF50" : "#F44336"
                    radius: 5
                }
                onClicked: {
                    bottomPanel.toggleHss()
                }
            }
            Button {
                text: "Toggle Camera"
                Layout.preferredWidth: 150
                background: Rectangle {
                    color: videoRect.visible ? "#4CAF50" : "#F44336"
                    radius: 5
                }
                onClicked: {
                    videoRect.visible = !videoRect.visible;
                }
            }
        }
        Item { Layout.fillWidth: true }

        ColumnLayout {
            Layout.alignment: Qt.AlignVCenter; spacing: 5
            Loader { sourceComponent: dataRow; property string title: "Voltage"; property string value: mockVoltage.toFixed(1) + " V" }
            Loader { sourceComponent: dataRow; property string title: "Current"; property string value: mockCurrent.toFixed(1) + " A" }
            Loader { sourceComponent: dataRow; property string title: "Battery"; property string value: mockBattery.toFixed(1) + " %" }
        }
    }
}