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
        title: qsTr("Tags")
    }

    contentHeader: UpdateHeader { }

    flickable: ListView {
        model: Tags
        delegate: TagDelegate {
            id: delegate
            tag: modelData
            onClicked: pageStack.push(tagPage, {tag: tag})
            onPressAndHold: {
                var menu = noteListMenu.createObject(root, {container: tag, delegate: delegate});
                menu.open();
            }
        }
    }

    Component {
        id: tagPage
        TagPage { }
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
