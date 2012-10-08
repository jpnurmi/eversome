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

CommonPage {
    id: root

    pageHeader: PageHeader {
        title: qsTr("Notebooks")
        busy: Manager.isBusy
        onRefresh: NoteStore.sync()
    }

    contentHeader: ContentHeader {
        text: qsTr("Last update: %1").arg(Qt.formatDateTime(NoteStore.currentTime).toString())
    }

    flickable: ListView {
        model: Notebooks
        delegate: NotebookDelegate {
            notebook: modelData
            onClicked: pageStack.push(noteListPage, {title: notebook.name, container: notebook})
            onPressAndHold: {
                var menu = noteListMenu.createObject(root, {container: notebook});
                menu.open();
            }
        }
    }

    Component {
        id: noteListPage
        NoteListPage { }
    }

    Component {
        id: noteListMenu
        NoteListMenu {
            id: menu
            onStatusChanged: {
               if (status === DialogStatus.Closing)
                    menu.destroy(1000);
            }
        }
    }
}
