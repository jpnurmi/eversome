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

Item {
    id: root

    property string link
    property alias source: image.source

    MouseArea {
        id: mouseArea
        property bool reallyPressed: pressed && containsMouse
        anchors.fill: parent
        onClicked: if (link) Qt.openUrlExternally(link)
    }

    Image {
        anchors.fill: parent
        source: "../images/listitem-pressed.png"

        Image {
            id: image
            anchors.fill: parent
            anchors.margins: UI.MEDIUM_SPACING
            fillMode: Image.PreserveAspectFit
        }

        Image {
            anchors.centerIn: parent
            source: image.status == Image.Null ? "../images/placeholder.png" : ""
        }
    }

    Image {
        anchors.top: parent.top
        anchors.right: parent.right
        source: "../images/external.png"
        visible: mouseArea.reallyPressed
    }
}
