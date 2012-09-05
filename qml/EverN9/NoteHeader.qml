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

Item {
    id: root

    property Note note

    width: parent.width
    height: column.height

    signal linkActivated(string link)

    Column {
        id: column

        anchors.left: parent.left
        anchors.right: parent.right
        anchors.leftMargin: UI.PAGE_MARGIN + UI.LARGE_SPACING
        anchors.rightMargin: UI.LARGE_SPACING

        spacing: UI.LARGE_SPACING

        Column {
            width: parent.width

            Item {
                width: parent.width
                height: UI.LARGE_SPACING
            }

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
                            value: modelData.name
                        }
                    }
                }
            }
        }

        Separator { }

        Label {
            id: contentLabel

            text: note ? note.content : ""
            font.pixelSize: UI.MEDIUM_FONT
            width: parent.width

            onLinkActivated: root.linkActivated(link)
        }
    }
}
