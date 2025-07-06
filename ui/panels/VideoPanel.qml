import QtQuick
import QtQuick.Shapes
import QtQuick.Controls.FluentWinUI3
import CustomTypes 1.0

Rectangle {
    id: videoRect
    visible: true
    width: 370
    implicitHeight: 240
    color: "black"

    VideoItem {
        id: video
        //opacity: 1
        height: 240
        anchors {
            //fill: parent
            //centerIn: videoRect
            leftMargin: 5
            rightMargin: 5
            topMargin: 5
            bottomMargin: 5
            top: videoRect.top
            bottom: videoRect.bottom
            left: videoRect.left
            right: videoRect.right
        }
    }
}