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
import QtWebKit 1.0
import com.nokia.meego 1.0
import com.evernote.types 1.0
import "UIConstants.js" as UI

CommonPage {
    id: root

    property Note note

    title: note ? note.title : ""

    flickable: Flickable {
        id: flickable

        contentWidth: webView.width
        contentHeight: webView.height

        PinchArea {
            property double startScale

            width: webView.width
            height: webView.height

            function zoom(scale) {
                var newScale = startScale + Math.log(scale) / Math.log(2);
                if (webView.width > flickable.width || newScale > startScale)
                    webView.contentsScale = newScale;
            }

            onPinchStarted: startScale = webView.contentsScale
            onPinchUpdated: zoom(pinch.scale)
            onPinchFinished: zoom(pinch.scale)

            WebView {
                id: webView

                url: note ? note.filePath : ""

                preferredWidth: flickable.width
                preferredHeight: flickable.height
                settings.defaultFontSize: UI.MEDIUM_FONT

                contentsScale: 1.0
                onDoubleClick: contentsScale = flickable.width / webView.width
            }
        }
    }

    onStatusChanged: {
        if (status == PageStatus.Activating) {
            if (note && !note.filePath)
                NoteStore.getNote(note.data());
        }
    }
}
