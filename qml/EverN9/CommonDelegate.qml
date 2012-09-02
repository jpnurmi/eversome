import QtQuick 1.1
import com.nokia.meego 1.0
import "UIConstants.js" as UI

BorderImage {
    id: root

    property bool highlighted: false
    property bool pressed: mouseArea.pressed && mouseArea.containsMouse
    default property alias data: container.data

    signal clicked()
    signal pressAndHold()

    width: parent.width
    height: container.height + 2 * UI.LARGE_SPACING

    border.top: 8
    border.left: 16
    border.bottom: 8

    source: highlighted && pressed ? "images/listitem-highlighted-pressed.png" :
            pressed ? "images/listitem-pressed.png" :
            highlighted ? "images/listitem-highlighted.png" : ""

    Item {
        id: container
        height: childrenRect.height
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.leftMargin: parent.border.left + UI.PAGE_MARGIN
        anchors.rightMargin: UI.PAGE_MARGIN
        anchors.verticalCenter: parent.verticalCenter
    }

    MouseArea {
        id: mouseArea
        anchors.fill: parent
        onClicked: root.clicked()
        onPressAndHold: root.pressAndHold()
    }

    Image {
        visible: index < root.ListView.view.count - 1
        source: "images/listitem-separator.png"
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.verticalCenter: parent.bottom
        anchors.leftMargin: UI.PAGE_MARGIN + UI.LARGE_SPACING
        anchors.rightMargin: UI.LARGE_SPACING
    }
}
