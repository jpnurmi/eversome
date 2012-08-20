import QtQuick 1.0
import com.nokia.meego 1.0

Item{
    property string statusText : qsTr("__title__")
    property string textColor: "white"
    height:75
    width: parent.width
    anchors.top:parent.top

    z:9
    Rectangle{
        width:parent.width
        height:parent.height
        color: "green"
    }

    Label{
        id: label
        text: statusText
        font.pixelSize: 36
        font.bold: false
        font.weight: Font.Light
        color: textColor
        anchors.verticalCenter: parent.verticalCenter
        x:20

    }
    Image{
        source: "images/header_bottom.png"
        anchors.bottom: parent.bottom
        width:900
    }
    BusyIndicator{
        id:progress;
        visible: false
        anchors.verticalCenter: parent.verticalCenter
        anchors.rightMargin: 30
        anchors.right: parent.right
        platformStyle: BusyIndicatorStyle { size: "medium" }
        running: true
    }
    function showProgress(){
        progress.visible = true
    }
    function hideProgress(){
        progress.visible = false
    }
}
