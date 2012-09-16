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

    property ItemModel notes

    flickable: ListView {
        model: notes
        delegate: NoteDelegate {
            highlighted: index % 2
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
        ContextMenu {
            id: menu

            property Note note

            MenuLayout {
                MenuItem {
                    text: qsTr("Edit")
                    onClicked: Qt.openUrlExternally("https://www.evernote.com/mobile/EditNote.action?noteGuid=" + note.guid)
                }
                MenuItem {
                    text: qsTr("Move")
                    onClicked: {
                        var dialog = notebookDialog.createObject(root, {note: note});
                        dialog.open();
                    }
                }
                MenuItem {
                    text: qsTr("Delete")
                    onClicked: NoteStore.deleteNote(note.data())
                }
            }

            onStatusChanged: {
               if (status === DialogStatus.Closing)
                    menu.destroy(1000);
            }
        }
    }

    Component {
        id: notebookDialog
        SelectionDialog {
            id: dialog

            property Note note

            onAccepted: {
                if (selectedIndex !== -1)
                    NoteStore.moveNote(note.data(), Notebooks.at(selectedIndex).data())
                dialog.destroy(1000);
            }

            onRejected: dialog.destroy(1000)

            Component.onCompleted: {
                for (var i = 0; i < Notebooks.count; ++i)
                    dialog.model.append({name: Notebooks.at(i).name})
            }
        }
    }
}
