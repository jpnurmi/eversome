import QtQuick 1.1
import com.nokia.meego 1.0
import com.evernote.types 1.0
import "UIConstants.js" as UI

CommonDelegate {
    id: root

    property Search search

    Label {
        id: nameLabel

        text: search ? search.name : ""
        font.pixelSize: UI.MEDIUM_FONT
        font.weight: Font.Bold
        wrapMode: Text.NoWrap
        elide: Text.ElideRight

        anchors.left: parent.left
        anchors.right: badge.left
        anchors.rightMargin: UI.MEDIUM_SPACING
    }

    Badge {
        id: badge

        value: search ? search.query : ""

        anchors.right: parent.right
        anchors.rightMargin: UI.PAGE_MARGIN
        anchors.verticalCenter: parent.verticalCenter
    }
}
