import QtQuick 2.5
import QtQuick.Controls 1.4
import QtLocation 5.14
import QtPositioning 5.14

Item {
    objectName:"rectangle"

    Plugin {
        id: mapPlugin
        name: "esri" // "mapboxgl", "esri", osm...
    }

    Map {
        anchors.fill: parent
        plugin: mapPlugin
        center: QtPositioning.coordinate(-31.16581,148.08496)
        zoomLevel: 7
        activeMapType:supportedMapTypes[1]

        MapQuickItem
        {
                    zoomLevel: 7
                    id: marker
                    coordinate: QtPositioning.coordinate(-31.16581,148.08496)
                    anchorPoint.x: image1.width/2
                    anchorPoint.y: image1.height/2
                    sourceItem: Image
                    {

                            id:image1
                            transformOrigin:Item.Center
                            source:"qrc:/assets/icons/rovicon2"
                            objectName:"markerimg"
                    }
            }

    }

}
