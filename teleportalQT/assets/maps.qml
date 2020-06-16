import QtQuick 2.5
import QtQuick.Controls 1.4
import QtLocation 5.6
import QtPositioning 5.5

Item {
    objectName:"rectangle"

    Plugin {
        id: mapPlugin
        name: "esri" // "mapboxgl", "esri", osm...
    }

    Map {
        anchors.fill: parent
        plugin: mapPlugin
        center: QtPositioning.coordinate(144.58,37.49)
        zoomLevel: 3
        activeMapType:supportedMapTypes[1]
    }
}
