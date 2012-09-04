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
