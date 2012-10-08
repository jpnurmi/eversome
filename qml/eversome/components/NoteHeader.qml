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
import com.evernote.types 1.0
import "../UIConstants.js" as UI

ContentHeader {
    id: root

    property Note note

    height: flow.height + 2 * UI.SMALL_SPACING

    Label {
        id: dateLabel
        text: note ? Qt.formatDateTime(note.updated) : ""
        font.pixelSize: UI.SMALL_FONT
        font.weight: Font.Light
        anchors.left: parent.left
        anchors.leftMargin: UI.PAGE_MARGIN + UI.LARGE_SPACING
        anchors.verticalCenter: parent.verticalCenter
    }

    Flow {
        id: flow
        anchors.left: dateLabel.right
        anchors.right: parent.right
        anchors.leftMargin: UI.MEDIUM_SPACING
        anchors.rightMargin: UI.MEDIUM_SPACING
        anchors.verticalCenter: parent.verticalCenter

        layoutDirection: Qt.RightToLeft
        spacing: UI.SMALL_SPACING

        Repeater {
            model: note ? note.tags : null
            Badge {
                value: modelData.name
            }
        }
    }
}
