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
import com.nokia.meego 1.1
import "UIConstants.js" as UI

Page {
    id: root

    signal menuRequested()

    TabGroup {
        id: tabGroup
        PageStack {
            id: notebooksTab
            objectName: "notebooks"
            NotebookListPage {
                id: notebooksPage
                tools: tabBar
            }
            Component.onCompleted: notebooksTab.push(notebooksPage)
        }
        PageStack {
            id: tagTab
            objectName: "tags"
            TagListPage {
                id: tagPage
                tools: tabBar
            }
            Component.onCompleted: tagTab.push(tagPage)
        }
        PageStack {
            id: searchTab
            objectName: "searcesh"
            SearchListPage {
                id: searchPage
                tools: tabBar
            }
            Component.onCompleted: searchTab.push(searchPage)
        }
    }

    tools: ToolBarLayout {
        id: tabBar
        ToolIcon {
            iconId: "toolbar-back"
            opacity: enabled ? 1.0 : UI.DISABLED_OPACITY
            enabled: !!tabGroup.currentTab && tabGroup.currentTab.depth > 1
            onClicked: tabGroup.currentTab.pop()
        }
        ButtonRow {
            exclusive: true
            TabButton {
                tab: notebooksTab
                iconSource: "image://theme/icon-m-toolbar-new-email"
            }
            TabButton {
                tab: tagTab
                iconSource: "image://theme/icon-m-toolbar-tag"
            }
            TabButton {
                tab: searchTab
                iconSource: "image://theme/icon-m-toolbar-search"
            }
        }
        ToolIcon {
            iconId: "toolbar-view-menu"
            onClicked: root.menuRequested()
        }
    }

    Component.onCompleted: {
        tabGroup.currentTab = notebooksTab
    }
}
