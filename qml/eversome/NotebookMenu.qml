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

    property Notebook notebook
    property NotebookDelegate delegate
    property PageHeader header

    MenuLayout {
        MenuItem {
            text: qsTr("Rename")
            enabled: !!delegate || !!header
            onClicked: {
                if (delegate)
                    delegate.edit(notebook.name);
                else if (header)
                    header.edit(notebook.name);
            }
        }
        MenuItem {
            text: !!notebook && notebook.unread ? qsTr("Mark all as read") : qsTr("Mark all as unread")
            onClicked: {
                notebook.unread = !notebook.unread;
                for (var i = 0; i < notebook.notes.count; ++i)
                    Database.saveNote(notebook.notes.at(i));
            }
        }
    }
}
