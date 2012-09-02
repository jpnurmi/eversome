import QtQuick 1.1
import com.nokia.meego 1.0
import "UIConstants.js" as UI

Image {
    id: root

    property alias title: label.text
    property alias busy: indicator.running

    source: "image://theme/color2-meegotouch-view-header-fixed"

    Text {
        id: label
        anchors.left: parent.left
        anchors.right: indicator.left
        anchors.verticalCenter: parent.verticalCenter
        anchors.leftMargin: UI.PAGE_MARGIN + UI.LARGE_SPACING
        anchors.rightMargin: UI.MEDIUM_SPACING
        font.weight: Font.Light
        font.family: UI.FONT_FAMILY
        font.pixelSize: UI.LARGE_FONT
        textFormat: Text.PlainText
        color: "white"
        elide: Text.ElideRight
    }

    BusyIndicator {
        id: indicator
        visible: running
        anchors.right: parent.right
        anchors.rightMargin: UI.LARGE_SPACING
        anchors.verticalCenter: parent.verticalCenter
        style: BusyIndicatorStyle { inverted: true }
    }
}
