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
    }

    contentHeader: UpdateHeader { }

    flickable: ListView {
        model: Notebooks
        delegate: NotebookDelegate {
            id: delegate
            notebook: modelData
            onClicked: pageStack.push(notebookPage, {notebook: notebook})
            onPressAndHold: {
                var menu = notebookMenu.createObject(root, {notebook: notebook, delegate: delegate});
                menu.open();
            }
        }
    }

    Component {
        id: notebookPage
        NotebookPage { }
    }

    Component {
        id: notebookMenu
        NotebookMenu {
            id: menu
            onStatusChanged: {
               if (status === DialogStatus.Closing)
                    menu.destroy(1000);
            }
        }
    }
}
