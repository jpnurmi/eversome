import QtQuick 1.1
import com.nokia.meego 1.0
import com.evernote.types 1.0
import "UIConstants.js" as UI

CommonDelegate {
    id: root

    property Notebook notebook

    Label {
        id: nameLabel

        text: notebook ? notebook.name : ""
        font.pixelSize: UI.MEDIUM_FONT
        font.weight: Font.Bold
        wrapMode: Text.NoWrap
        elide: Text.ElideRight

        anchors.left: parent.left
        anchors.right: badge.left
        anchors.rightMargin: UI.MEDIUM_SPACING
    }

    Label {
        id: dateLabel

        text: notebook ? Qt.formatDateTime(notebook.updated, qsTr("yyyy-MM-dd hh:mm")) : ""
        font.pixelSize: UI.SMALL_FONT
        font.weight: Font.Light
        wrapMode: Text.NoWrap

        anchors.top: nameLabel.bottom
        anchors.left: parent.left
        anchors.right: badge.left
        anchors.topMargin: UI.TINY_SPACING
        anchors.rightMargin: UI.MEDIUM_SPACING
    }

    Badge {
        id: badge

        value: notebook ? notebook.notes.count : 0

        anchors.right: parent.right
        anchors.rightMargin: UI.PAGE_MARGIN
        anchors.verticalCenter: parent.verticalCenter
    }
}
