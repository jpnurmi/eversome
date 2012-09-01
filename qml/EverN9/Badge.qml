import QtQuick 1.1
import com.nokia.meego 1.0
import "UIConstants.js" as UI

BorderImage {
    id: root

    property alias value: label.text

    width: Math.max(label.width + 16, 25)
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
