import QtQuick
import QtQuick.Controls
Item {
    id: root
    width: 120
    height: 120

    property real value: 0.0
    property real from: 0.0
    property real to: 100.0
    property string title: "Title"
    property string unit: ""
    property int decimals: 1
    property int majorTickCount: 4 // How many segments to divide the scale into

    readonly property real angleRange: 270
    readonly property real startAngle: -225
    readonly property real valueAngle: startAngle + (value - from) / (to - from) * angleRange

    // --- SCALE LABELS ---
    Repeater {
        model: root.majorTickCount + 1
        delegate: Text {
            readonly property real tickRatio: index / root.majorTickCount
            readonly property real angle: root.startAngle + tickRatio * root.angleRange
            readonly property real value: root.from + tickRatio * (root.to - from)

            text: value.toFixed(0)
            color: "#A0A0A0"
            font.pixelSize: 10
            font.weight: Font.Bold

            // Position the text in a circle just outside the gauge arc
            x: root.width / 2 + (root.width / 2 - 5) * Math.cos(angle * Math.PI / 180) - width / 2
            y: root.height / 2 + (root.height / 2 - 5) * Math.sin(angle * Math.PI / 180) - height / 2
        }
    }

    Canvas {
        id: canvas
        anchors.fill: parent
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

            // 1. Draw background arc
            ctx.beginPath();
            ctx.arc(centerX, centerY, radius, startAngle * Math.PI / 180, (startAngle + angleRange) * Math.PI / 180);
            ctx.lineWidth = 8;
            ctx.strokeStyle = "#424242";
            ctx.stroke();

            // 2. Draw value arc
            if (value > from) {
                ctx.beginPath();
                ctx.arc(centerX, centerY, radius, startAngle * Math.PI / 180, valueAngle * Math.PI / 180);
                ctx.lineWidth = 8;
                ctx.strokeStyle = "#00AEEF";
                ctx.lineCap = "round";
                ctx.stroke();
            }
        }
    }

    Rectangle {
        id: needle
        width: root.width / 2 - 30
        height: 2
        color: "white"
        antialiasing: true
        anchors.verticalCenter: parent.verticalCenter
        anchors.left: parent.horizontalCenter
        transformOrigin: Item.Left
        rotation: valueAngle
        Behavior on rotation { NumberAnimation { duration: 300; easing.type: Easing.InOutCubic } }
    }

    Column {
        anchors.centerIn: parent
        spacing: 2
        Label {
            text: root.value.toFixed(root.decimals); color: "white"
            font.pixelSize: 24; font.weight: Font.Bold
            anchors.horizontalCenter: parent.horizontalCenter
        }
        Label {
            text: root.title + " (" + root.unit + ")"; color: "#A0A0A0"
            font.pixelSize: 11
            anchors.horizontalCenter: parent.horizontalCenter
        }
    }
}