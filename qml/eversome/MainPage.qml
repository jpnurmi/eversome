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
            property Item currentPage: !!tabGroup.currentTab ? tabGroup.currentTab.currentPage : undefined
            iconId: "toolbar-view-menu"
            onClicked: !!currentPage && !!currentPage.menu ? currentPage.menu.open() : aboutMenu.open()
        }
    }

    Menu {
        id: aboutMenu
        MenuLayout {
            MenuItem {
                text: qsTr("About")
                onClicked: aboutDialog.open()
            }
        }
    }

    QueryDialog {
        id: aboutDialog
        acceptButtonText: qsTr("OK")
        titleText: qsTr(AppTitle)
        message: qsTr("<h3>Evernote client for Nokia N9</h3>" +
                      "<p>Copyright (C) 2012 J-P Nurmi <a href=\"mailto:jpnurmi@gmail.com\">jpnurmi@gmail.com</a></p>" +
                      "<p>Remember everything.<br/>Capture anything.<br/>Access anywhere.<br/>Find things fast.</p>" +
                      "<p><img src='%1'/></p>" +
                      "<p>Use <a href='http://www.evernote.com'>Evernote</a> to save your ideas, things you see, and things you like. Then find them all on any computer, phone or device you use. For free.</p>" +
                      "<p><small>This program is free software; you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation; either version 2 of the License, or (at your option) any later version.</small></p>" +
                      "<p><small>This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.</small></p>")
                      .arg(Qt.resolvedUrl("images/banner.png"))
        onLinkActivated: Qt.openUrlExternally(link)
    }

    Component.onCompleted: {
        tabGroup.currentTab = notebooksTab
    }
}
