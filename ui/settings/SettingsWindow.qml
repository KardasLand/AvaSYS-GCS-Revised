import QtQuick
import QtQuick.Controls.FluentWinUI3
import QtQuick.Layouts

import App.Context 1.0

ApplicationWindow {
    id: settingsWindow
    width: 500; height: 500
    visible: true
    title: qsTr("AvaSYS GCS Settings")
    background: Rectangle { color: "#1e1e1e" }
    property var teknofestClient: AppContext.teknofestClient
    property var mavlinkClient: AppContext.mavlinkManager

    function saveSettings(){
        var url = serverUrlField.text.trim();
        if (!url.startsWith("http://") && !url.startsWith("https://")) {
            url = "https://" + url; // Default to HTTPS if no protocol is specified
        }


        // todo save settings
        teknofestClient.serverProperties.teknofestAuthProperty.setUsername(usernameField.text);
        teknofestClient.serverProperties.teknofestAuthProperty.setPassword(passwordField.text);
        teknofestClient.serverProperties.setUrl(url);

        if (enableQrCodeCheckBox.checked) {
            // convert text to double
            // AppContext.setTeknofestQRCode(
            //     parseFloat(qrCodeLatitudeField.text),
            //     parseFloat(qrCodeLongitudeField.text)
            // );
            teknofestClient.serverProperties.teknofestQRCodeProperty.setQrLatitude(parseFloat(qrCodeLatitudeField.text));
            teknofestClient.serverProperties.teknofestQRCodeProperty.setQrLongitude(parseFloat(qrCodeLongitudeField.text));
        } else {
            // Disable QR Code by setting latitude and longitude to -1
            teknofestClient.serverProperties.teknofestQRCodeProperty.setQrLatitude(-1);
            teknofestClient.serverProperties.teknofestQRCodeProperty.setQrLongitude(-1);
        }
        var mavlinkUrl = mavlinkUrlField.text.trim().split(':');
        console.log("MAVLink URL:", mavlinkUrl[0], mavlinkUrl[1]);
        AppContext.mavlinkManager.setMavlinkSettings(
            mavlinkUrl[0], // Protocol (e.g., "udp")
            parseInt(mavlinkUrl[1]) // Port (e.g., "14550")
        );
        console.log("Settings saved successfully. Saved data logs:");
        console.log("Username:", usernameField.text);
        console.log("Password :", passwordField.text);
        console.log("Server URL:", url);
        console.log("MAVLink UDP URL:", mavlinkUrlField.text);
        console.log("MAVLink Video URL:", mavlinkVideoUrlField.text);
        console.log("QR Code Latitude:", qrCodeLatitudeField.text);
        console.log("QR Code Longitude:", qrCodeLongitudeField.text);
        console.log("QR Code Enabled:", enableQrCodeCheckBox.checked);
        console.log("MAVLink Settings:", mavlinkUrlField.text);
    }

    header: ToolBar {
        id: header
        RowLayout {
            anchors.right: parent.right
            spacing: 10
            Button {
                text: settingsWindow.teknofestClient.statusString === "Connected." ? qsTr("Disconnect") : qsTr("Connect")
                onClicked:{
                    if (settingsWindow.teknofestClient.statusString === "Connected.") {
                        settingsWindow.teknofestClient.disconnectFromServer();
                    } else {
                        saveSettings();
                        settingsWindow.teknofestClient.login();
                    }
                }
            }
            Button {
                text: settingsWindow.mavlinkClient.connectionStatusString !== "Disconnected" ? qsTr("Disconnect MAVLink") : qsTr("Connect MAVLink")
                onClicked: {
                    if (settingsWindow.mavlinkClient.connectionStatusString !== "Disconnected") {
                        settingsWindow.mavlinkClient.disconnect();
                    } else {
                        var mavlinkUrl = mavlinkUrlField.text.trim().split(':');
                        if (mavlinkUrl.length === 2) {
                            saveSettings();
                            settingsWindow.mavlinkClient.connectFromSettings();
                        } else {
                            console.error("Invalid MAVLink URL format. Expected format: 'protocol:port'");
                        }
                    }
                }
            }
            Button {
                text: qsTr("Save Settings")
                onClicked: {
                    saveSettings();
                    settingsWindow.close();
                }
            }
            Button {
                text: qsTr("Close")
                onClicked: settingsWindow.close()
            }
        }
    }

    Timer {
        interval: 200; running: true; repeat: true
        onTriggered: {
            // This polling pattern is robust if the teknofestClient can appear/disappear
            if (settingsWindow.teknofestClient !== AppContext.teknofestClient) {
                settingsWindow.teknofestClient = AppContext.teknofestClient;
            }
        }
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
                    text: settingsWindow.teknofestClient.serverProperties.teknofestAuthProperty.username || "" // Placeholder for username
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
                    text: settingsWindow.teknofestClient.serverProperties.teknofestAuthProperty.password || "" // Placeholder for password
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
                    text: settingsWindow.teknofestClient.serverProperties.url || "https://teknofest.neostellar.net" // Placeholder for server URL
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
                        text: settingsWindow.mavlinkClient.host + ":" + settingsWindow.mavlinkClient.port // Placeholder for MAVLink URL
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
                    checked: settingsWindow.teknofestClient.serverProperties.teknofestQRCodeProperty.qrLatitude !== -1 && settingsWindow.teknofestClient.serverProperties.teknofestQRCodeProperty.qrLongitude !== -1
                    // enabled: settingsWindow.teknofestClient.getTeknofestQRCodeProperty_latitude() !== -1
                }
                Label {
                    text: qsTr("QR Code Latitude:")
                    color: "#A0A0A0"
                    font.pixelSize: 14
                }
                TextField {
                    id: qrCodeLatitudeField
                    placeholderText: qsTr("QR Code Latitude")
                    text: settingsWindow.teknofestClient.serverProperties.teknofestQRCodeProperty.qrLatitude || "0.0" // Placeholder for latitude
                    Layout.fillWidth: true
                    enabled: enableQrCodeCheckBox.checked
                }
                Label {
                    text: qsTr("QR Code Longitude:")
                    color: "#A0A0A0"
                    font.pixelSize: 14
                }
                TextField {
                    id: qrCodeLongitudeField
                    placeholderText: qsTr("QR Code Longitude")
                    text: settingsWindow.teknofestClient.serverProperties.teknofestQRCodeProperty.qrLongitude || "0.0" // Placeholder for longitude
                    Layout.fillWidth: true
                    enabled: enableQrCodeCheckBox.checked
                }
            }
            Label {
                text: qsTr("About")
                font.bold: true
                color: "#A0A0A0"
                font.pixelSize: 14
            }
            Text {
                text: qsTr("AvaSYS GCS Revised\nVersion 2.0\n© 2025 Neo Stellar")
                color: "#A0A0A0"
                font.pixelSize: 12
                wrapMode: Text.Wrap
            }
            Item { Layout.fillHeight: true }
        }
    }
}