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
    busy: Sync.active || contentLabel.openingLink

    flickable: Flickable {
        anchors.fill: parent
        anchors.margins: UI.LARGE_SPACING
        anchors.leftMargin: UI.PAGE_MARGIN + UI.LARGE_SPACING
        contentHeight: column.height

        Column {
            id: column

            width: parent.width
            spacing: UI.LARGE_SPACING

            Row {
                width: parent.width
                spacing: UI.MEDIUM_SPACING

                Label {
                    id: dateLabel
                    text: note ? Qt.formatDateTime(note.updated, qsTr("yyyy-MM-dd hh:mm")) : ""
                    font.pixelSize: UI.SMALL_FONT
                    font.weight: Font.Light
                    wrapMode: Text.NoWrap
                }

                Flow {
                    width: parent.width - dateLabel.width - parent.spacing
                    layoutDirection: Qt.RightToLeft
                    spacing: UI.SMALL_SPACING

                    Repeater {
                        model: note ? note.tags : null
                        Badge {
                            value: modelData
                        }
                    }
                }
            }

            Separator { }

            Label {
                id: contentLabel

                property bool openingLink: false

                text: note ? note.content : ""
                font.pixelSize: UI.MEDIUM_FONT
                width: parent.width

                onLinkActivated: {
                    openingLink = true;
                    Qt.openUrlExternally(link);
                }

                Connections {
                    target: Qt.application
                    onActiveChanged: {
                        if (!Qt.application.active)
                            contentLabel.openingLink = false;
                    }
                }
            }

            Repeater {
                model: note ? note.resources : null
                Image {
                    source: modelData
                }
            }
        }
    }

    onStatusChanged: {
        if (status == PageStatus.Activating) {
            if (note && !note.content)
                Sync.fetch(note.note());
        }
    }
}
