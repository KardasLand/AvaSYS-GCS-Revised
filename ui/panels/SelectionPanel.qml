// will display the selected plane information on left side of the screen
import QtQuick
import QtQuick.Controls.FluentWinUI3
import QtQuick.Layouts
import App.Context 1.0

Rectangle {
    id: selectionPanel2
    height: 168
    width: 250
    color: "#212121"
    radius: 10
    opacity: 0.8
    property var selectedVehicle: AppContext.vehicleManager.selectedVehicle
    visible: selectedVehicle !== null && selectedVehicle !== undefined
    ColumnLayout {

        // anchors.fill: parent
        // anchors.top: parent.top
        anchors.horizontalCenter: parent.horizontalCenter
        // anchors.centerIn: parent
        spacing: 15


        Item { Layout.fillHeight: true }
        ColumnLayout {
            Text {
                text: "Vehicle Information"
                color: "white"
                font.pixelSize: 16
                font.bold: true
                horizontalAlignment: Text.AlignHCenter
            }
            Layout.alignment: Qt.AlignVCenter; spacing: 5
            Component { id: infoRow; Row { spacing: 8; Label { text: title + ":"; color: "#A0A0A0"; font.pixelSize: 14; width: 75; horizontalAlignment: Text.AlignRight } Label { text: value; color: "white"; font.pixelSize: 14; font.weight: Font.Bold } } }
            Loader { sourceComponent: infoRow; property string title: "Vehicle ID"; property string value: selectedVehicle.displayId || "N/A" }
            Loader { sourceComponent: infoRow; property string title: "Vehicle Type"; property string value: selectedVehicle.dataSource === 1 ? "MavLink" : "Teknofest" }
        }
        ColumnLayout {
            Text {
                text: "Vehicle Location"
                color: "white"
                font.pixelSize: 16
                font.bold: true
                horizontalAlignment: Text.AlignHCenter
            }
            Layout.alignment: Qt.AlignVCenter; spacing: 5
            Component { id: locationRow; Row { spacing: 8; Label { text: title + ":"; color: "#A0A0A0"; font.pixelSize: 14; width: 75; horizontalAlignment: Text.AlignRight } Label { text: value; color: "white"; font.pixelSize: 14; font.weight: Font.Bold } } }
            Loader { sourceComponent: locationRow; property string title: "Latitude"; property string value: selectedVehicle.coordinate.latitude.toFixed(6) + "°" || "N/A" }
            Loader { sourceComponent: locationRow; property string title: "Longitude"; property string value: selectedVehicle.coordinate.longitude.toFixed(6) + "°" || "N/A" }
            Loader { sourceComponent: locationRow; property string title: "Altitude"; property string value: selectedVehicle.altitude.toFixed(2) + " m" || "N/A" }
            Loader { sourceComponent: locationRow; property string title: "Ground Speed"; property string value: selectedVehicle.groundSpeed.toFixed(2) + " m/s"  || "N/A" }
        }
        ColumnLayout {
            Text {
                text: "Vehicle Axis"
                color: "white"
                font.pixelSize: 16
                font.bold: true
                horizontalAlignment: Text.AlignHCenter
            }
            Layout.alignment: Qt.AlignVCenter; spacing: 5
            Component { id: axisRow; Row { spacing: 8; Label { text: title + ":"; color: "#A0A0A0"; font.pixelSize: 14; width: 75; horizontalAlignment: Text.AlignRight } Label { text: value; color: "white"; font.pixelSize: 14; font.weight: Font.Bold } } }
            Loader { sourceComponent: axisRow; property string title: "Roll"; property string value: selectedVehicle.roll.toFixed(2) + "°" || "N/A" }
            Loader { sourceComponent: axisRow; property string title: "Pitch"; property string value: selectedVehicle.pitch.toFixed(2) + "°" || "N/A" }
            Loader { sourceComponent: axisRow; property string title: "Yaw"; property string value: selectedVehicle.heading.toFixed(2) + "°" || "N/A" }
        }
        ColumnLayout {
            Text {
                text: "Vehicle Hardware"
                color: "white"
                font.pixelSize: 16
                font.bold: true
                horizontalAlignment: Text.AlignHCenter
            }
            Layout.alignment: Qt.AlignVCenter; spacing: 5
            Component { id: hardwareRow; Row { spacing: 8; Label { text: title + ":"; color: "#A0A0A0"; font.pixelSize: 14; width: 75; horizontalAlignment: Text.AlignRight } Label { text: value; color: "white"; font.pixelSize: 14; font.weight: Font.Bold } } }
            Loader { sourceComponent: hardwareRow; property string title: "Battery Voltage"; property string value: selectedVehicle.batteryVoltage.toFixed(2) + " V" || "N/A" }
            Loader { sourceComponent: hardwareRow; property string title: "Battery Current"; property string value: selectedVehicle.batteryCurrent.toFixed(2) + " A" || "N/A" }
            Loader { sourceComponent: hardwareRow; property string title: "Battery Remaining"; property string value: selectedVehicle.batteryRemaining.toFixed(2) + " %" || "N/A" }
        }

        Item { Layout.fillHeight: true }
    }
}