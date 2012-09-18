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
import com.nokia.meego 1.1
import com.nokia.extras 1.0
import "UIConstants.js" as UI

PageStackWindow {
    id: window

    initialPage: MainPage {
        onMenuRequested: menu.open()

        Menu {
            id: menu
            MenuLayout {
                MenuItem {
                    text: qsTr("About")
                    onClicked: aboutDialog.open()
                }
            }
        }

        QueryDialog {
            id: aboutDialog
            acceptButtonText: qsTr("OK")
            titleText: qsTr("Eversome v0.0.3") // TODO
            message: qsTr("<h3>Evernote client for Nokia N9</h3>" +
                          "<p>Copyright (C) 2012 J-P Nurmi <a href=\"mailto:jpnurmi@gmail.com\">jpnurmi@gmail.com</a></p>" +
                          "<p>Remember everything.<br/>Capture anything.<br/>Access anywhere.<br/>Find things fast.</p>" +
                          "<p><img src='%1'/></p>" +
                          "<p>Use <a href='http://www.evernote.com'>Evernote</a> to save your ideas, things you see, and things you like. Then find them all on any computer, phone or device you use. For free.</p>" +
                          "<p><small>This program is free software; you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation; either version 2 of the License, or (at your option) any later version.</small></p>" +
                          "<p><small>This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.</small></p>")
                          .arg(Qt.resolvedUrl("images/banner.png"))
            onLinkActivated: Qt.openUrlExternally(link)
        }

        InfoBanner {
            id: banner

            Connections {
                target: Session
                onEstablished: {
                    banner.text = qsTr("Logged in as %1").arg(Session.userName)
                    banner.show();
                }
                onError: {
                    banner.text = error;
                    banner.show();
                }
            }

            Connections {
                target: NoteStore
                onError: {
                    banner.text = error;
                    banner.show();
                }
            }
        }
    }

    Component.onCompleted: theme.colorScheme = 2
}
