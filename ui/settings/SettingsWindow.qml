import QtQuick
import QtQuick.Controls.FluentWinUI3
import QtQuick.Layouts

import App.Context 1.0

ApplicationWindow {
    id: settingsWindow
    width: 500; height: 600
    visible: true
    title: qsTr("AvaSYS GCS Settings")
    background: Rectangle { color: "#1e1e1e" }
    property var teknofestClient: AppContext.teknofestClient
    property var mavlinkClient: AppContext.mavlinkManager
    property var isSerialCommunication: true // Default to Serial Communication

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



        // check if url is port:baudrate or protocol:port
        if (mavlinkUrl.length !== 2) {
            console.error("Invalid MAVLink URL format. Expected format: 'protocol:port' or 'port:baudrate'");
            return;
        }
        if (settingsWindow.mavlinkClient.communicationType === 0) {
            // MAVLink UDP
            AppContext.mavlinkManager.setMavlinkUdpSettings(
                mavlinkUrl[0], // Protocol (e.g., "udp")
                parseInt(mavlinkUrl[1]) // Port (e.g., "14550")
            );
        } else {
            // MAVLink Serial
            AppContext.mavlinkManager.setSerialPort(
                mavlinkUrl[0] // Serial port (e.g., "/dev/ttyUSB0")
            );
            AppContext.mavlinkManager.setBaudrate(
                parseInt(mavlinkUrl[1]) // Baudrate (e.g., "57600")
            );
        }
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
            if (settingsWindow.mavlinkClient !== AppContext.mavlinkManager) {
                settingsWindow.mavlinkClient = AppContext.mavlinkManager;
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
                id: mavlinkSettingsLayout
                Label {
                    text: qsTr("MAVLink Settings")
                    font.bold: true
                    color: "#A0A0A0"
                    font.pixelSize: 14
                }
                ColumnLayout {
                    spacing: 10

                    Label {
                        text: qsTr("MAVLink Communication Type:")
                        color: "#A0A0A0"
                        font.pixelSize: 14
                    }
                    TabBar {
                        implicitWidth: mavlinkSettingsLayout.width
                        id: mavlinkTabBar
                        currentIndex: settingsWindow.mavlinkClient.communicationType
                        onCurrentIndexChanged: {
                            settingsWindow.mavlinkClient.set_comm_type_qml(currentIndex);
                        }
                        TabButton {
                            text: qsTr("UDP")
                            onClicked: {
                                settingsWindow.mavlinkClient.set_comm_type_qml(0);
                            }
                        }
                        TabButton {
                            text: qsTr("Serial")
                            onClicked: {
                                settingsWindow.mavlinkClient.set_comm_type_qml(1);
                            }
                        }
                    }

                    Label {
                        text: settingsWindow.mavlinkClient.communicationType === 0 ? qsTr("MAVLink UDP Settings") : qsTr("MAVLink Serial Settings")
                        color: "#A0A0A0"
                        font.pixelSize: 14
                    }
                    TextField {
                        id: mavlinkUrlField
                        placeholderText: settingsWindow.mavlinkClient.communicationType === 0 ? qsTr("MAVLink UDP URL (e.g., 127.0.0.1:14550)") : qsTr("MAVLink Serial Link with Baudrate (e.g., /dev/ttyUSB0:56700)")
                        text: {
                            if (settingsWindow.mavlinkClient.communicationType === 0) {
                                // return udp
                                return settingsWindow.mavlinkClient.host + ":" + settingsWindow.mavlinkClient.port;
                            }else {
                                // return serial
                                return settingsWindow.mavlinkClient.serialPort + ":" + settingsWindow.mavlinkClient.baudrate;
                            }
                        }
                        Layout.fillWidth: true
                    }
                    Label {
                        text: qsTr("Vehicle Video Port:")
                        color: "#A0A0A0"
                        font.pixelSize: 14
                    }
                    TextField {
                        id: mavlinkVideoUrlField
                        placeholderText: qsTr("MAVLink Video Port")
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