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
        title: qsTr("Search")
        busy: Manager.isBusy
        onRefresh: Cloud.sync()
    }

    contentHeader: UpdateHeader { }

    flickable: ListView {
        model: Searches
        delegate: SearchDelegate {
            id: delegate
            search: modelData
            onClicked: {
                Cloud.search(search.data());
                pageStack.push(noteListPage, {title: search.name, container: search})
            }
            onPressAndHold: {
                var menu = noteListMenu.createObject(root, {container: search, delegate: delegate});
                menu.open();
            }
        }
    }

    Component {
        id: noteListPage
        NoteListPage {
            // TODO: pool: SearchPool
        }
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
