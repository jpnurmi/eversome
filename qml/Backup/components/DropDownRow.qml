import QtQuick 1.0

import QtQuick 1.0
import "../constants.js" as Constants
import com.nokia.meego 1.0
import com.nokia.extras 1.0


Rectangle{

    property alias title: lblTitle.text
    property alias selectedText: lblSelectedItem.text
    signal clicked();

    id:base
    width: parent.width
    height : 60
    color:"transparent"
    MouseArea{
        anchors.fill: parent
        onPressed: {
            base.color = "lightgray"
        }
        onReleased: {
            base.color = "transparent"
        }
        onClicked: {
            base.clicked();
        }
    }

    Label{
        id:lblTitle
        text:title
        anchors.margins: Constants.DEFAILT_MARGIN
        anchors.verticalCenter: parent.verticalCenter
        anchors.left: parent.left
        font.pixelSize: Constants.FONT_NORMAL_SIZE

    }
    Label{
        id:lblSelectedItem
        font.bold: true
        font.pixelSize: Constants.FONT_NORMAL_SIZE
        text:selectedText
        anchors.margins: Constants.DEFAILT_MARGIN
        anchors.verticalCenter: parent.verticalCenter
        anchors.left: lblTitle.right
    }
    Image {
        source: "../images/icon-m-common-combobox-arrow.png"
        anchors.verticalCenter: parent.verticalCenter
        anchors.left: lblSelectedItem.right
        anchors.margins: Constants.DEFAILT_MARGIN

    }
}
