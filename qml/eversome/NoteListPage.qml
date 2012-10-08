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

CommonPage {
    id: root

    property QtObject container

    menu: NoteListMenu {
        id: menu
        parent: root
        container: root.container
    }

    flickable: ListView {
        model: container ? container.notes : 0
        delegate: NoteDelegate {
            note: modelData
            onClicked: pageStack.push(notePage, {note: modelData})
            onPressAndHold: {
                var menu = noteMenu.createObject(root, {note: modelData});
                menu.open();
            }
        }
    }

    Component {
        id: notePage
        NotePage { }
    }

    Component {
        id: noteMenu
        NoteMenu {
            id: menu
            onMoving: {
                var dialog = notebookDialog.createObject(root, {note: note});
                dialog.open();
            }
            onStatusChanged: {
               if (status === DialogStatus.Closing)
                    menu.destroy(1000);
            }
        }
    }

    Component {
        id: notebookDialog
        NotebookDialog {
            id: dialog
            onAccepted: dialog.destroy(1000)
            onRejected: dialog.destroy(1000)
        }
    }
}