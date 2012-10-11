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

TextField {
    id: editor

    signal edited(string text)

    function edit(text) {
        editor.text = text;
        editor.forceActiveFocus();
        editor.openSoftwareInputPanel();
    }

    Keys.onEnterPressed: {
        root.edited(text);
        editor.destroy();
    }

    Keys.onReturnPressed: {
        root.edited(text);
        editor.destroy();
    }

    Keys.onEscapePressed: {
        editor.destroy();
    }

    onActiveFocusChanged: {
        if (!activeFocus)
            editor.destroy();
    }

    InverseMouseArea {
        anchors.fill: parent
        onPressedOutside: editor.destroy()
    }

    Component.onDestruction: editor.closeSoftwareInputPanel()
}
