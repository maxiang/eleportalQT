import QtQuick 2.0

import org.freedesktop.gstreamer.GLVideoItem 1.0

//Rectangle {
Item {
    objectName:"rectangle"
    Item {
        anchors.fill: parent

        GstGLVideoItem {
            id: video
            objectName: "videoItem"
            anchors.centerIn: parent
            width: parent.width
            height: parent.height
        }
        Text
         {
           id:tlabel
           objectName:"videoLabel"
           text: ""
           font.pointSize: 22
           font.bold: true
           font.family:"Arial Black"
           color: "red"
           horizontalAlignment: Text.AlignHCenter
           x:parent.width/2-contentWidth/2
           y:10

         }
    }
}
