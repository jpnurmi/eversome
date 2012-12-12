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
import "UIConstants.js" as UI
import "components"

CommonDelegate {
    id: root

    property Tag tag

    highlighted: !!tag && tag.unread

    onEdited: {
        tag.name = text;
        Cloud.renameTag(tag.data());
    }

    Label {
        id: nameLabel

        text: tag ? tag.name : ""
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

        value: tag ? tag.notes.count : ""

        anchors.right: parent.right
        anchors.rightMargin: UI.PAGE_MARGIN
        anchors.verticalCenter: parent.verticalCenter
    }
}
