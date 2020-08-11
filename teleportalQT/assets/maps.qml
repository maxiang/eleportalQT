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
        id:mapview
        objectName:"qmlMapView"
        anchors.fill: parent
        plugin: mapPlugin
        center: QtPositioning.coordinate(-31.16581,148.08496)
        zoomLevel:7
        property int defzoomLevel:7
        property int curzoomLevel:7
        property int maxzoomLevel:17
        property int  interval:1
        property bool scalestate: false
        activeMapType:supportedMapTypes[1]

        Timer {
                  id: scaleTimer
                  interval: 1000;
                  running: false;
                  repeat: true
                  objectName:"qmlTimer"
                  onTriggered:
                  {

                      if(!mapview.scalestate)
                      {
                          if(mapview.zoomLevel<mapview.maxzoomLevel)
                          {
                              mapview.zoomLevel+=mapview.interval;
                          }
                          else
                          {
                              mapview.scalestate=true;
                              mapview.zoomLevel-=mapview.interval;
                          }
                      }

                      else
                      {
                          if(mapview.zoomLevel>mapview.defzoomLevel)
                          {
                              mapview.zoomLevel-=mapview.interval;
                          }
                          else
                          {
                              mapview.scalestate=false;
                              mapview.zoomLevel+=mapview.interval;
                          }
                      }

                  }
              }
        MouseArea{
            anchors.fill: parent
            acceptedButtons: Qt.LeftButton| Qt.RightButton;
            onClicked:{
               // if(scaleTimer.running)
               // {
                    scaleTimer.stop();
               // }
               // else
               // {
                 //   scaleTimer.start();
               // }
            }
        }

        Component.onCompleted:
        {
           scaleTimer.start();
        }

        MapQuickItem
        {
                    zoomLevel: 0
                    id: marker
                    coordinate: QtPositioning.coordinate(-31.16581,148.08496)
                    anchorPoint.x: image1.width/2
                    anchorPoint.y: image1.height/2
                    objectName:"markerItem"
                    sourceItem: Grid{
                    Image
                    {

                            id:image1
                            transformOrigin:Item.Center
                            source:"qrc:/assets/icons/rovicon2"
                            objectName:"markerimg"
                            scale:0.2
                    }
                    }
            }

    }

}
