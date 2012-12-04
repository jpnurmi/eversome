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

Menu {
    id: menu

    property Note note
    property NoteDelegate delegate
    property PageHeader header

    signal moving

    MenuLayout {
        MenuItem {
            text: qsTr("Edit")
            onClicked: Qt.openUrlExternally("https://www.evernote.com/mobile/EditNote.action?noteGuid=" + note.guid)
        }
        MenuItem {
            text: qsTr("Rename")
            enabled: !!delegate || !!header
            onClicked: {
                if (delegate)
                    delegate.edit(note.title);
                else if (header)
                    header.edit(note.title);
            }
        }
        MenuItem {
            text: qsTr("Move to")
            onClicked: menu.moving();
        }
        MenuItem {
            text: !!note && note.unread ? qsTr("Mark as read") : qsTr("Mark as unread")
            onClicked: {
                note.unread = !note.unread;
                Database.saveNote(note);
            }
        }
        MenuItem {
            text: qsTr("Delete")
            onClicked: NotePool.destroy(note.data())
        }
    }
}
