import QtQuick
import QtPositioning
import QtLocation
import QtQuick.Controls

import App.Context 1.0

Rectangle {
    id: root
    color: "black"

    property var vehicleModel: AppContext.vehicleManager.vehicles
    property var mainVehicle: null

    Timer {
        interval: 200; running: true; repeat: true
        onTriggered: {
            // This polling pattern is robust if the mainVehicle can appear/disappear
            if (root.mainVehicle !== AppContext.vehicleManager.mainVehicle) {
                root.mainVehicle = AppContext.vehicleManager.mainVehicle;
            }
        }
    }

    // Map component is declared first, so it's at the back
    MapView {
        id: mapvi
        anchors.fill: parent
        map.plugin: mapPlugin
        map.center: QtPositioning.coordinate(47.3979722, 8.546166666666666)
        map.zoomLevel: 14
        map.copyrightsVisible: true
        map.onSupportedMapTypesChanged: {
            if (map.supportedMapTypes.length > 0) {
                map.activeMapType = map.supportedMapTypes[map.supportedMapTypes.length - 1];
            }
        }

        MapItemView {
            model: root.vehicleModel
            parent: mapvi.map // This line made me lose my fucking mind. Spent 5 hours trying to figure out why the map items were not showing up.
            delegate: MapQuickItem {
                id: mapItem
                visible: true // Ensure the item is visible
                property var plane: modelData;
                coordinate: QtPositioning.coordinate(modelData.coordinate.latitude, modelData.coordinate.longitude)
                sourceItem: Item {
                    id: planeItem
                    transform: Rotation {
                        id: markerRotation
                        origin.x: planeItem.width / 2
                        origin.y: planeItem.height / 2
                        angle: plane.heading // Use the vehicle's heading for rotation;
                    }

                    Image {
                        id: planeImage
                        anchors.centerIn: parent
                        source: (plane.isSelected) ? "../assets/plane_selected.png" : "../assets/plane.png"
                        width: 50
                        height: 50
                        MouseArea {
                            width: parent.width
                            height: parent.height
                            onClicked: {
                                //console.log(planeController.planes[index].isSelected)
                                //planeController.changeSelection((getPlane().sysid === -1) ? getPlane().teamid : getPlane().sysid);
                                AppContext.vehicleManager.selectVehicle(plane.vehicleId);
                                console.log("Selected vehicle with ID:", plane.vehicleId);
                                console.log("Selected?:", plane.isSelected);
                            }
                        }
                    }

                    Rectangle {
                        width: 2
                        height: 100
                        color: "black"
                        x: planeImage.x + planeImage.width / 2
                        y: planeImage.y - 32 + planeImage.height
                        transform: Rotation {
                            origin.x: width / 2
                            origin.y: height
                            angle: 180
                        }
                    }

                    width: planeItem.width * 0.5
                    height: planeItem.height * 0.5

                    Text {
                        text: plane.displayId
                        anchors.centerIn: parent
                    }
                }
            }
        }
    }

    // --- SANITY CHECK: A regular ListView ---
    ListView {
        id: listView
        visible: false
        width: 250
        height: 40
        anchors.left: parent.left
        anchors.bottom: parent.bottom
        anchors.margins: 10
        interactive: false

        model: root.vehicleModel

        delegate: Text {
            color: "yellow"
            font.pointSize: 12
            text: "ListView OK! ID: " + modelData.heading
        }

        Rectangle {
            anchors.fill: parent
            color: "black"
            border.color: "gray"
            z: -1
        }
    }

    Plugin{
        id: mapPlugin
        name: "osm"
        PluginParameter {
            name: "osm.useragent"
            value: "AvaSYS GCS/2.0"
        }
        PluginParameter { name: "osm.mapping.providersrepository.disabled"; value: "true" }
        PluginParameter { name: "osm.mapping.custom.host"; value: "http://tile.thunderforest.com/atlas/%z/%x/%y.jpg90?apikey="+THUNDERFOREST_API_KEY }
    }
}