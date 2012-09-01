import QtQuick 1.1
import com.nokia.meego 1.0
import com.evernote.types 1.0
import "UIConstants.js" as UI

CommonDelegate {
    id: root

    property Note note

    Label {
        id: nameLabel

        text: note ? note.title : ""
        font.pixelSize: UI.MEDIUM_FONT
        font.weight: Font.Bold
        wrapMode: Text.NoWrap
        elide: Text.ElideRight

        anchors.left: parent.left
        anchors.right: tags.left
        anchors.rightMargin: UI.MEDIUM_SPACING
    }

    Label {
        id: dateLabel

        text: note ? Qt.formatDateTime(note.updated, qsTr("yyyy-MM-dd hh:mm")) : ""
        font.pixelSize: UI.SMALL_FONT
        font.weight: Font.Light
        wrapMode: Text.NoWrap

        anchors.top: nameLabel.bottom
        anchors.left: parent.left
        anchors.right: tags.left
        anchors.topMargin: UI.TINY_SPACING
        anchors.rightMargin: UI.MEDIUM_SPACING
    }

    Flow {
        id: tags

        flow: Flow.TopToBottom
        spacing: UI.SMALL_SPACING
        layoutDirection: Qt.RightToLeft
        height: nameLabel.height + dateLabel.height + UI.TINY_SPACING

        anchors.right: parent.right
        anchors.rightMargin: UI.PAGE_MARGIN

        Repeater {
            id: repeater
            model: note ? note.tags : null
            Badge {
                value: modelData
            }
        }
    }
}
