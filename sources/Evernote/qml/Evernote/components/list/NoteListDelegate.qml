import QtQuick 1.0
import "../../constants.js" as Constants
import com.nokia.meego 1.0

DefaultDelegate{

    Rectangle{
        id: cachedRect
        color: "green"
        width: cached ? 0 : 8
        height: parent.height-60
        anchors.left: parent.left
        anchors.verticalCenter: parent.verticalCenter
        anchors.leftMargin: 8
    }
    Label {
        anchors.top: parent.top
        anchors.topMargin: 30
        anchors.left: cachedRect.right
        anchors.leftMargin: 12
        text: "<strong>"+title+"</stong>"
        width:parent.width-x
        wrapMode: Text.WordWrap
    }
    Label{
        text:dateCreated
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 30
        anchors.left: cachedRect.right
        anchors.leftMargin: 12
        color: cached ? "gray" : "green"
        font.pixelSize: 20
        font.weight: Font.Light
    }
    Image{
        source: "../../images/header_bottom.png"
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.leftMargin: 8
        width:parent.width-16
        visible: (index+1 != listModel.count) && (listModel.get(index).dateCreated == listModel.get(index+1).dateCreated)
    }

}
