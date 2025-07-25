import QtQuick
import QtQuick.Window
import QtQuick.Controls
import QtQuick.Layouts
import App.Context 1.0
import io.smth 1.0

BorderImage {
    anchors {
        top: parent.top
        right: parent.right
    }
    id: horizonPanel
    property int a: mainPanel.height - bottomPanel1.height
    width: 460;
    height: {
        if (a > 460){
            return 460;
        }else {
            return a;
        }
    }

    Component.onCompleted: {
        // Initialize the mainVehicle property
        //console.log("height:", a, "width:", width);
        //console.log(mainPanel.height, bottomPanel1.height);
    }


    property var mainVehicle: AppContext.vehicleManager.mainVehicle;
    visible: plane !== null
    Rectangle {
        anchors.fill: parent
        //anchors.margins: 20
        radius: 10; color: "#111"
        opacity: {
            return 0.9
        }
    }

    Timer {
        interval: 200; running: true; repeat: true
        onTriggered: {
            // This polling pattern is robust if the mainVehicle can appear/disappear
            if (horizonPanel.mainVehicle !== AppContext.vehicleManager.mainVehicle) {
                horizonPanel.mainVehicle = AppContext.vehicleManager.mainVehicle;
            }
        }
    }

    ArtificialHorizon {
        id: artificialHorizon
        width: 200; height: 280
        anchors.centerIn: parent
        anchors.horizontalCenterOffset: -10

        roll: horizonPanel.mainVehicle.roll !== null ? horizonPanel.mainVehicle.roll : 0 //3.2
        pitch: horizonPanel.mainVehicle.pitch !== null ? horizonPanel.mainVehicle.pitch : 0
    }

    Text {
        id: altitudeActualLabel
        anchors.horizontalCenter: artificialHorizon.horizontalCenter
        anchors.bottom: artificialHorizon.bottom; anchors.bottomMargin: 10
        text: Math.ceil(altitudeLinearIndicator.current)
        font { bold: true; pointSize: 14 }
        color: "#ebb751"
    }

    TickMark {
        anchors.left: artificialHorizon.right; anchors.leftMargin: 5
        anchors.top: artificialHorizon.top; anchors.topMargin: 90
    }

    TickMark {
        anchors.right: artificialHorizon.left; anchors.rightMargin: 5
        anchors.top: artificialHorizon.top; anchors.topMargin: 90
        transform: [ Scale { origin.x: 3; xScale: -1 } ]
    }

    LinearIndicator {
        id: headingLinearIndicator
        visible: horizonPanel.a >= 460
        width: artificialHorizon.width; height: 40
        anchors.top: artificialHorizon.bottom; anchors.topMargin: 20
        anchors.horizontalCenter: parent.horizontalCenter

        current: {
            return horizonPanel.mainVehicle.heading !== null ? horizonPanel.mainVehicle.heading : 0
        }
        wrap: 360

        font {
            family: "Helvetica"
            pixelSize: 16
        }
    }

    LinearIndicator {
        id: speedLinearIndicator
        width: 60; height: artificialHorizon.height
        anchors.right: artificialHorizon.left; anchors.rightMargin: 30
        anchors.verticalCenter: artificialHorizon.verticalCenter

        current: {
            return horizonPanel.mainVehicle.groundSpeed !== null ? horizonPanel.mainVehicle.groundSpeed : 0
        }

        orientation: LinearIndicator.VerticalRight
        tickmarksStepSize: 40
        tickmarksStepValue: 5
        tickmarksEach: 5

        segments: [
            ColorSegment { from: 0; to: 100; color: "red" },
            ColorSegment { from: 100; to: 115; color: "yellow" },
            ColorSegment { from: 320; to: 400; color: "red" }
        ]

        font {
            family: "Helvetica"
            pixelSize: 16
        }
    }

    Text {
        id: speedTargetLabel
        anchors.horizontalCenter: speedLinearIndicator.horizontalCenter
        anchors.top: speedLinearIndicator.bottom; anchors.topMargin: 5
        text: horizonPanel.mainVehicle.groundSpeed !== null ? horizonPanel.mainVehicle.groundSpeed.toFixed(2) : 0 //Math.ceil(speedDial.position * 400)
        font { bold: true; pointSize: 14 }
        color: "white"
    }

    LinearIndicator {
        id: altitudeLinearIndicator
        width: 60; height: artificialHorizon.height
        anchors.left: artificialHorizon.right; anchors.leftMargin: 30
        anchors.verticalCenter: artificialHorizon.verticalCenter

        orientation: LinearIndicator.VerticalLeft
        tickmarksStepSize: 40
        tickmarksStepValue: 100
        tickmarksEach: 200

        current: {
            //console.log(horizonPanel.mainVehicle.altitude)
            return (horizonPanel.mainVehicle.altitude !== null) ? horizonPanel.mainVehicle.altitude * 100 : 0
        }

        font {
            family: "Helvetica"
            pixelSize: 16
        }
    }

    Canvas {
        // vertical speed instead of airspeed
        property real diff: (horizonPanel.mainVehicle.groundSpeed !== null ? horizonPanel.mainVehicle.groundSpeed : 0) / 1.5
        property real fullScale: 1.0
        anchors.right: parent.right; anchors.rightMargin: 30
        anchors.verticalCenter: parent.verticalCenter
        width: 10; height: 250
        onDiffChanged: requestPaint()
        onPaint: {
            var ctx = getContext("2d")
            ctx.clearRect(0, 0, width, height)
            ctx.save()
            ctx.translate(0, height / 2)
            ctx.strokeStyle = 'white'
            ctx.lineWidth = 2
            var hc = height / 2 - 2
            var yy = [0, hc / 3, hc * 2 / 3, hc * 5 / 6, hc]
            yy.forEach(function(item, i, yy) {
                ctx.beginPath(); ctx.moveTo(0, item); ctx.lineTo(5, item); ctx.stroke();
                ctx.beginPath(); ctx.moveTo(0, -item); ctx.lineTo(5, -item); ctx.stroke();
            });
            ctx.restore()

            ctx.strokeStyle = 'green'
            ctx.lineWidth = 4
            var diffBoxed = diff
            if (diffBoxed > fullScale)  diffBoxed =  fullScale
            if (diffBoxed < -fullScale) diffBoxed = -fullScale;
            ctx.save()
            ctx.translate(width / 2, height / 2 - (diffBoxed / fullScale) * hc)
            ctx.beginPath()
            ctx.moveTo(-width / 2, 0); ctx.lineTo(width / 2, 0)
            ctx.stroke()
            ctx.restore()
        }
    }

    Text {
        id: altitudeTargetLabel
        anchors.horizontalCenter: altitudeLinearIndicator.horizontalCenter
        anchors.top: altitudeLinearIndicator.bottom; anchors.topMargin: 5
        text: horizonPanel.mainVehicle.altitude !== null ? (horizonPanel.mainVehicle.altitude * 100) : 0; //Math.ceil(altitudeDial.position * 10000)
        font { bold: true; pointSize: 14 }
        color: "white"
    }

    Text {
        id: aThrLabel
        anchors.left: parent.left; anchors.top: parent.top
        anchors.margins: 30
        text: "A/THR"; color: "#00d0d0"; font { pixelSize: 18 }
        visible: true//athrButton.pressed
    }

    Text {
        id: spdLabel
        anchors.top: aThrLabel.bottom
        anchors.horizontalCenter: aThrLabel.horizontalCenter
        text: "SPD"; color: "#00d0d0"; font { pixelSize: 18 }
        visible: true
    }

    Text {
        id: hdgLabel
        anchors.top: aThrLabel.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        text: "HDG"; color: "#00d0d0"; font { pixelSize: 18 }
    }

    Text {
        id: apLabel
        anchors.top: parent.top; anchors.topMargin: 30
        anchors.right: parent.right; anchors.rightMargin: 80
        text: "A/P"; color: "#00d0d0"; font { pixelSize: 18 }
    }

    Text {
        id: altLabel
        anchors.top: apLabel.bottom
        anchors.right: apLabel.left
        text: "ALT"; color: "#00d0d0"; font { pixelSize: 18 }
    }

    Text {
        id: vsLabel
        anchors.top: apLabel.bottom
        anchors.left: apLabel.right
        text: "V/S"; color: "#00d0d0"; font { pixelSize: 18 }
    }
}