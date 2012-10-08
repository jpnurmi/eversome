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
import "../UIConstants.js" as UI

BorderImage {
    id: root

    property alias text: label.text

    signal refresh()

    source: "image://theme/meegotouch-calendar-eventinvitation-background"

    Text {
        id: label
        anchors.fill: parent
        verticalAlignment: Text.AlignVCenter
        anchors.leftMargin: UI.PAGE_MARGIN + UI.LARGE_SPACING
        anchors.rightMargin: UI.MEDIUM_SPACING
        font.weight: Font.Light
        font.family: UI.FONT_FAMILY
        font.pixelSize: UI.SMALL_FONT
        textFormat: Text.PlainText
    }
}
