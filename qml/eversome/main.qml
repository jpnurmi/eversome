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
                target: Manager
                onError: {
                    banner.text = error;
                    banner.show();
                }
            }
        }
    }

    Component.onCompleted: theme.colorScheme = 2
}
