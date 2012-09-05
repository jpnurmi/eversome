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
import "UIConstants.js" as UI

BorderImage {
    id: root

    property alias value: label.text

    visible: value != ""
    width: Math.max(label.width + 20, 26)
    height: label.height + 8
    source: "images/countindicator.png"
    border { top: 12; left: 12; right: 12; bottom: 12 }

    Label {
        id: label
        color: "white"
        font.pixelSize: UI.TINY_FONT
        anchors.centerIn: parent
    }
}
