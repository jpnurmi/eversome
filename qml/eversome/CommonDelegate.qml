/*
* Copyright (C) 2012 J-P Nurmi <jpnurmi@gmail.com>
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*/
import QtQuick 1.1
import com.nokia.meego 1.0
import "UIConstants.js" as UI
import "components"

BorderImage {
    id: root

    property bool highlighted: false
    property bool pressed: mouseArea.pressed && mouseArea.containsMouse
    default property alias data: container.data

    signal clicked()
    signal pressAndHold()
    signal edited(string text)

    function edit(text) {
        var editor = editorComponent.createObject(root);
        editor.edit(text);
    }

    width: parent.width
    height: preferredHeight

    property real preferredHeight: container.height + 2 * UI.LARGE_SPACING

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

    Separator {
        visible: index < root.ListView.view.count - 1
        anchors.verticalCenter: parent.bottom
        anchors.leftMargin: UI.PAGE_MARGIN + UI.LARGE_SPACING
        anchors.rightMargin: UI.LARGE_SPACING
    }

    Component {
        id: editorComponent
        InlineEditor {
            anchors.left: container.left
            anchors.right: container.right
            anchors.verticalCenter: container.verticalCenter
        }
    }
}
