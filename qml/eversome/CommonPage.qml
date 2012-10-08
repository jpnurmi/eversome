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
import "components"

Page {
    id: root

    property Menu menu
    property Header header
    property Footer footer
    property Flickable flickable

    Item {
        id: flickableParent
        anchors.top: footer ? footer.bottom : header ? header.bottom : root.top
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom

        ScrollDecorator {
            id: scroller
        }
    }

    onHeaderChanged: {
        if (header) {
            header.parent = root;
            header.anchors.top = root.top;
            header.anchors.left = root.left;
            header.anchors.right = root.right;
        }
    }

    onFooterChanged: {
        if (footer) {
            footer.parent = root;
            footer.anchors.left = root.left;
            footer.anchors.right = root.right;
            footer.anchors.top = function() { return header ? header.bottom : root.top; }
        }
    }

    onFlickableChanged: {
        if (flickable) {
            flickable.parent = flickableParent;
            flickable.anchors.fill = flickableParent;
        }
        scroller.flickableItem = flickable;
    }
}
