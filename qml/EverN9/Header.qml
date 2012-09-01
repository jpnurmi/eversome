import QtQuick 1.1
import com.nokia.meego 1.0
import "UIConstants.js" as UI

Image {
    id: root

    property alias title: label.text

    source: "image://theme/color2-meegotouch-view-header-fixed"

    Text {
        id: label
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.verticalCenter: parent.verticalCenter
        anchors.margins: UI.PAGE_MARGIN + UI.LARGE_SPACING
        font.weight: Font.Light
        font.family: UI.FONT_FAMILY
        font.pixelSize: UI.LARGE_FONT
        textFormat: Text.PlainText
        color: "white"
        elide: Text.ElideRight
    }
}
