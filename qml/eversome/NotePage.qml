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
import QtWebKit 1.1
import com.nokia.meego 1.0
import com.evernote.types 1.0
import "UIConstants.js" as UI
import "components"

CommonPage {
    id: root

    property Note note

    pageHeader: PageHeader {
        id: header
        title: !!note ? note.title : ""
        onEdited: {
            note.title = text;
            Cloud.renameNoe(note.data());
        }
        onRefresh: Cloud.fetchNote(note.data())
    }

    contentHeader: NoteHeader {
        note: root.note
    }

    menu: NoteMenu {
        id: menu
        parent: root
        note: root.note
        header: header
        onMoving: {
            var dialog = notebookDialog.createObject(root, {note: note});
            dialog.open();
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

    flickable: FlickableWebView {
        id: flickable

        webView.url: note ? note.filePath : ""
        webView.backgroundColor: "transparent"

        webView.javaScriptWindowObjects: QtObject {
            WebView.windowObjectName: "qml"
            function openUrlExternally(url) {
                Qt.openUrlExternally(url);
            }
        }

        webView.settings.pluginsEnabled: true
        webView.settings.javascriptEnabled: true
        webView.settings.printElementBackgrounds: false
        webView.settings.defaultFontSize: UI.MEDIUM_FONT
    }

//    Repeater {
//        model: note ? note.resources : null
//        Item {
//            id: resource
//            function update() {
//                if (note.filePath && modelData.filePath && modelData.hash && modelData.fileName)
//                    flickable.webView.evaluateJavaScript("handleResource('" + modelData.fileName + "', '" + modelData.hash + "', '" + modelData.filePath + "', '" + modelData.thumbnail + "')");
//            }
//            Connections {
//                target: modelData
//                onFileNameChanged: resource.update()
//                onFilePathChanged: resource.update()
//                onThumbnailChanged: resource.update()
//            }
//            Connections {
//                target: note
//                onFilePathChanged: resource.update()
//            }
//            Connections {
//                target: flickable.webView
//                onLoadFinished: resource.update()
//            }
//        }
//    }

    Connections {
        target: note
        onResourcesChanged: flickable.webView.reload.trigger()
    }

    onNoteChanged: {
        if (note) {
            if (!note.filePath)
                Cloud.fetchNote(note.data());
            if (note.unread) {
                note.unread = false;
                Database.saveNote(note);
            }
        }
    }
}
