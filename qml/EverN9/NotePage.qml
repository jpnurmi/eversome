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

    property Note note

    title: note ? note.title : ""
    busy: Manager.isBusy || openingLink

    property bool openingLink: false

    flickable: ListView {

        header: NoteHeader {
            id: header

            note: root.note

            onLinkActivated: {
                root.openingLink = true;
                Qt.openUrlExternally(link);
            }

            Connections {
                target: Qt.application
                onActiveChanged: {
                    if (!Qt.application.active)
                        root.openingLink = false;
                }
            }
        }

        model: note ? note.resources : null

        delegate: Item {
            width: parent.width / 2
            height: parent.width / 2

            Thumbnail {
                anchors.fill: parent
                anchors.margins: UI.PAGE_MARGIN + UI.LARGE_SPACING
                source: modelData.thumbnail
                link: modelData.filePath
            }
        }
    }

    onStatusChanged: {
        if (status == PageStatus.Activating) {
            if (note && note.isEmpty)
                NoteStore.getNote(note.note());
        }
    }
}
