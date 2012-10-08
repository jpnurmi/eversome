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

    title: note ? note.title : ""

    menu: NoteMenu {
        id: menu
        parent: root
        note: root.note
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

    Repeater {
        model: note ? note.resources : null
        Item {
            Connections {
                target: modelData
                onFilePathChanged: {
                    if (modelData.filePath)
                        flickable.webView.evaluateJavaScript("handleResource('foobar', '" + modelData.hash + "', '" + modelData.filePath + "')");
                }
            }
        }
    }

    onNoteChanged: {
        if (note) {
            if (!note.filePath)
                NoteStore.getNote(note.data());
            if (note.unread) {
                note.unread = false;
                Database.saveNote(note);
            }
        }
    }
}
