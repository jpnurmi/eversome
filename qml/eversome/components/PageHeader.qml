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
import "../UIConstants.js" as UI

Image {
    id: root

    property alias title: label.text
    property bool busy: Manager.isBusy

    signal refresh()
    signal edited(string text)

    function edit(text) {
        var editor = editorComponent.createObject(root);
        editor.edit(text);
    }

    source: "../images/header.png"

    Text {
        id: label
        anchors.left: parent.left
        anchors.right: loader.left
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

    Loader {
        id: loader
        width: 32
        anchors.right: parent.right
        anchors.rightMargin: UI.LARGE_SPACING
        anchors.verticalCenter: parent.verticalCenter
        sourceComponent: root.busy ? busyIndicator : refreshIcon
        Component {
            id: busyIndicator
            BusyIndicator {
                running: true
                style: BusyIndicatorStyle { inverted: true }
            }
        }
        Component {
            id: refreshIcon
            ToolIcon {
                iconSource: "image://theme/icon-s-refresh-inverse"
                onClicked: {
                    SyncPool.sync();
                    root.refresh();
                }
            }
        }
    }

    Component {
        id: editorComponent
        InlineEditor {
            anchors.left: label.left
            anchors.right: loader.right
            anchors.verticalCenter: parent.verticalCenter
        }
    }
}
