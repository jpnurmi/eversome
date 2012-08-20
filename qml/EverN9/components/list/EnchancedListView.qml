import QtQuick 1.1
import com.nokia.meego 1.0
import "../../constants.js" as Constants

ListView {
    anchors.bottom: parent.bottom
    anchors.left: parent.left
    anchors.right: parent.right
    focus: true
    highlightMoveSpeed: Constants.LIST_VIEW_HIGHLIGHT_SPEED
    highlight: DefaultHighlight{}
}

