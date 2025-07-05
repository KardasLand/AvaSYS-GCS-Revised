import QtQuick
import QtQuick.Controls.FluentWinUI3
import QtQuick.Layouts

import App.Context 1.0

ApplicationWindow {
    id: settingsWindow
    width: 460; height: 460
    visible: true
    title: qsTr("AvaSYS GCS Settings")
    background: Rectangle { color: "#1e1e1e" }

    header: ToolBar {
        id: header
        RowLayout {
            anchors.right: parent.right
            spacing: 10
            Button {
                text: qsTr("Save and Close")
                onClicked: {
                    // todo save settings
                    console.log(qsTr("Settings saved!"));
                    settingsWindow.close();
                }
            }
            Button {
                text: qsTr("Close")
                onClicked: settingsWindow.close()
            }
        }
    }

    property var teknofestClient: AppContext.teknofestClient
    Component.onCompleted: {
        console.log(qsTr("Settings Window Initialized"));
    }

    RowLayout {
        anchors.top: header.bottom
        anchors.fill: parent
        anchors.margins: 10
        spacing: 10
        ColumnLayout {
            spacing: 10
            Label {
                text: qsTr("Teknofest Client Settings")
                color: "#A0A0A0"
                font.bold: true
                font.pixelSize: 14
            }
            ColumnLayout {
                spacing: 10
                Label {
                    text: qsTr("Username:")
                    color: "#A0A0A0"
                    font.pixelSize: 14
                }
                TextField {
                    id: usernameField
                    placeholderText: qsTr("Username")
                    text: settingsWindow.teknofestClient.getTeknofestAuthProperty_username()
                    Layout.fillWidth: true
                }
                Label {
                    text: qsTr("Password:")
                    color: "#A0A0A0"
                    font.pixelSize: 14
                }
                TextField {
                    id: passwordField
                    placeholderText: qsTr("Password")
                    echoMode: TextInput.Password
                    text: settingsWindow.teknofestClient.getTeknofestAuthProperty_password()
                    Layout.fillWidth: true
                }
                Label {
                    text: qsTr("Server URL:")
                    color: "#A0A0A0"
                    font.pixelSize: 14
                }
                TextField {
                    id: serverUrlField
                    placeholderText: qsTr("Server URL")
                    text: settingsWindow.teknofestClient.getUrl() || "https://teknofest.neostellar.com" // Placeholder for server URL
                    Layout.fillWidth: true
                }
            }
            ColumnLayout {
                spacing: 10
                Label {
                    text: qsTr("MAVLink Settings")
                    font.bold: true
                    color: "#A0A0A0"
                    font.pixelSize: 14
                }
                ColumnLayout {
                    spacing: 10
                    Label {
                        text: qsTr("MAVLink UDP URL:")
                        color: "#A0A0A0"
                        font.pixelSize: 14
                    }
                    TextField {
                        id: mavlinkUrlField
                        placeholderText: qsTr("MAVLink UDP URL")
                        text: "udp://:14550" // Placeholder for MAVLink UDP URL
                        Layout.fillWidth: true
                    }
                    Label {
                        text: qsTr("MAVLink Video URL:")
                        color: "#A0A0A0"
                        font.pixelSize: 14
                    }
                    TextField {
                        id: mavlinkVideoUrlField
                        placeholderText: qsTr("MAVLink Video URL")
                        text: "11234" // Placeholder for video URL
                        Layout.fillWidth: true
                    }
                }
            }
            Item { Layout.fillHeight: true }
        }
        ColumnLayout {
            anchors.topMargin: 20
            spacing: 10
            Label {
                text: qsTr("QR Code Settings")
                font.bold: true
                color: "#A0A0A0"
                font.pixelSize: 14
            }
            ColumnLayout {
                spacing: 10
                CheckBox {
                    id: enableQrCodeCheckBox
                    text: qsTr("Enable QR Code")
                    checked: true // Default to enabled
                }
                Label {
                    text: qsTr("QR Code Latitude:")
                    color: "#A0A0A0"
                    font.pixelSize: 14
                }
                TextField {
                    id: qrCodeLatitudeField
                    placeholderText: qsTr("QR Code Latitude")
                    text: settingsWindow.teknofestClient.getTeknofestQRCodeProperty_latitude() || "0.0" // Placeholder for latitude
                    Layout.fillWidth: true
                }
                Label {
                    text: qsTr("QR Code Longitude:")
                    color: "#A0A0A0"
                    font.pixelSize: 14
                }
                TextField {
                    id: qrCodeLongitudeField
                    placeholderText: qsTr("QR Code Longitude")
                    text: settingsWindow.teknofestClient.getTeknofestQRCodeProperty_longitude() || "0.0" // Placeholder for longitude
                    Layout.fillWidth: true
                }
            }
            Label {
                text: qsTr("About")
                font.bold: true
                color: "#A0A0A0"
                font.pixelSize: 14
            }
            Text {
                text: qsTr("AvaSYS GCS 2.0\nVersion 1.0\n© 2023 AvaSYS Team")
                color: "#A0A0A0"
                font.pixelSize: 12
                wrapMode: Text.Wrap
            }
            Item { Layout.fillHeight: true }
        }
    }
}