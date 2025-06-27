// file: ui/components/RightPanel.qml
import QtQuick
import QtPositioning
import QtLocation
import QtQuick.Controls

import App.Context 1.0
import App.Models 1.0

Rectangle {
    color: "black"

    Plugin{
        id: mapPlugin
        name: "osm" // Use the "osm" plugin for OpenStreetMap
        PluginParameter {
            name: "osm.mapping.providersrepository.disabled"
            value: "true"
        }
        PluginParameter {
            name: "osm.mapping.custom.host";
            value: qsTr("http://tile.thunderforest.com/atlas/%z/%x/%y.jpg90?apikey=10a4757dce9c464b802a34190a616c8a")
        }
        PluginParameter {
            name: "osm.mapping.providersrepository.address"
            value: "http://maps-redirect.qt.io/osm/5.6/"
        }
        PluginParameter {
            name: "osm.mapping.highdpi_tiles"
            value: "true"
        }

    }

    MapView {
        id: mapvi
        anchors.fill: parent
        map.plugin: mapPlugin
        map.onSupportedMapTypesChanged: {
            map.activeMapType = map.supportedMapTypes[map.supportedMapTypes.length - 1];
        }
        map.center: QtPositioning.coordinate(37.53, -122.31)
        map.zoomLevel: 12
        Component.onCompleted: {
            map.clearData();
        }

        MapItemView {
            model: AppContext.vehicleManager.vehicles
            delegate: MapQuickItem {
                coordinate: modelData.coordinate
                anchorPoint.x: sourceItem.width / 2
                anchorPoint.y: sourceItem.height / 2

                sourceItem: Item {
                    width: 40
                    height: 40
                    rotation: modelData.heading

                    Rectangle {
                        id: planeIcon
                        anchors.centerIn: parent
                        width: 40
                        height: 40
                        color: "red"
                        // source: modelData.isSelected ? "qrc:/AvaSYS/ui/assets/plane_selected.png" : "qrc:/AvaSYS/ui/assets/plane.png"
                    }

                    Label {
                        text: modelData.displayId
                        color: "red"
                        font.bold: true
                        // background: Rectangle { color: "rgba(0,0,0,0.7)" }
                        anchors.horizontalCenter: parent.horizontalCenter
                        y: parent.height
                    }

                    MouseArea {
                        anchors.fill: parent
                        onClicked: AppContext.vehicleManager.selectVehicle(modelData.vehicleId)
                    }
                }
            }
        }
    }

    // MapView {
    //     id: mapView
    //     anchors.fill: parent
    //     map.plugin: Plugin {
    //         name: "osm"
    //         PluginParameter {
    //             name: "osm.mapping.custom.host";
    //             value: "http://tile.thunderforest.com/cycle/%z/%x/%y.png?apikey=10a4757dce9c464b802a34190a616c8a"
    //         }
    //     }
    //     map.onSupportedMapTypesChanged: {
    //         map.activeMapType = map.supportedMapTypes[map.supportedMapTypes.length - 1]
    //     }
    // }
    // Map {
    //     id: map
    //     anchors.fill: parent
    //     plugin: mapPlugin
    //     center: QtPositioning.coordinate(41.0, 29.0) // Istanbul
    //     zoomLevel: 10
    //
    //     Component.onCompleted: {
    //         map.clearData();
    //     }
    //
    //     // This is the core of the new vehicle display system.
    //     // MapItemView {
    //     //     model: AppContext.vehicleManager.vehicles
    //     //     delegate: MapQuickItem {
    //     //         coordinate: modelData.coordinate
    //     //         anchorPoint.x: sourceItem.width / 2
    //     //         anchorPoint.y: sourceItem.height / 2
    //     //
    //     //         sourceItem: Item {
    //     //             width: 40
    //     //             height: 40
    //     //             rotation: modelData.heading
    //     //
    //     //             Rectangle {
    //     //                 id: planeIcon
    //     //                 anchors.centerIn: parent
    //     //                 width: 40
    //     //                 height: 40
    //     //                 color: "red"
    //     //                 // source: modelData.isSelected ? "qrc:/AvaSYS/ui/assets/plane_selected.png" : "qrc:/AvaSYS/ui/assets/plane.png"
    //     //             }
    //     //
    //     //             Label {
    //     //                 text: modelData.displayId
    //     //                 color: "red"
    //     //                 font.bold: true
    //     //                 // background: Rectangle { color: "rgba(0,0,0,0.7)" }
    //     //                 anchors.horizontalCenter: parent.horizontalCenter
    //     //                 y: parent.height
    //     //             }
    //     //
    //     //             MouseArea {
    //     //                 anchors.fill: parent
    //     //                 onClicked: AppContext.vehicleManager.selectVehicle(modelData.vehicleId)
    //     //             }
    //     //         }
    //     //     }
    //     // }
    // }
}