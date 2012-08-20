import QtQuick 1.1
import com.nokia.meego 1.0

import com.nokia.extras 1.0

Rectangle {
    anchors.fill: parent
    z:11
    color: "#7bffffff";
    BusyIndicator{
        id:progress;
        visible: true
        anchors.verticalCenter: parent.verticalCenter
        anchors.horizontalCenter: parent.horizontalCenter
        platformStyle: BusyIndicatorStyle { size: "large" }
        running: true
    }
    visible: false;
    function show(){
        visible = true;
    }
    function close(){
        visible = false;
    }
}
