import QtQuick
import QtQuick.Controls

Item {
    id: root
    width: 120
    height: 120

    property real value: 0.0 // Heading value 0-360
    property string title: "Heading"
    property string unit: "°"

    Canvas {
        id: canvas
        anchors.fill: parent
        anchors.bottomMargin: 10
        antialiasing: true

        Connections {
            target: root
            function onValueChanged() { canvas.requestPaint(); }
        }

        onPaint: {
            var ctx = getContext("2d");
            ctx.reset();
            var centerX = width / 2;
            var centerY = height / 2;
            var radius = Math.min(centerX, centerY) - 15;

            // 1. Draw full background circle
            ctx.beginPath();
            ctx.arc(centerX, centerY, radius, 0, 2 * Math.PI);
            ctx.lineWidth = 8;
            ctx.strokeStyle = "#424242";
            ctx.stroke();

            // 2. Draw tiny value indicator on the circle
            ctx.beginPath();
            var valueAngle = (value - 90) * Math.PI / 180;
            ctx.arc(centerX, centerY, radius, valueAngle - 0.1, valueAngle + 0.1);
            ctx.lineWidth = 8;
            ctx.strokeStyle = "#00AEEF";
            ctx.lineCap = "round";
            ctx.stroke();

            // 3. Draw cardinal direction text
            ctx.fillStyle = "#A0A0A0";
            ctx.font = "bold 10px sans-serif";
            ctx.textAlign = "center";
            ctx.textBaseline = "middle";
            ctx.fillText("N", centerX, centerY - radius);
            ctx.fillText("E", centerX + radius, centerY);
            ctx.fillText("S", centerX, centerY + radius);
            ctx.fillText("W", centerX - radius, centerY);
        }
    }

    // --- Arrow Pointer ---
    Rectangle {
        id: arrow
        width: 2
        height: root.height / 2 - 20
        color: "white"
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottom: parent.verticalCenter
        transformOrigin: Item.Bottom
        rotation: root.value

        Behavior on rotation {
            RotationAnimation { duration: 300; easing.type: Easing.InOutCubic; direction: RotationAnimation.Shortest }
        }
    }

    Column {
        anchors.centerIn: parent
        spacing: 2
        Label {
            text: root.value.toFixed(0); color: "white"
            font.pixelSize: 22; font.weight: Font.Bold
            anchors.horizontalCenter: parent.horizontalCenter
        }
        Label {
            text: root.title + " (" + root.unit + ")"; color: "#A0A0A0"
            font.pixelSize: 9
            anchors.horizontalCenter: parent.horizontalCenter
        }
    }
}