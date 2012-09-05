import QtQuick 1.1
import com.nokia.meego 1.0

Page {
    id: root

    property alias busy: header.busy
    property alias title: header.title
    property Flickable flickable

    Item {
        id: flickableParent
        anchors.top: header.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom

        ScrollDecorator {
            id: scroller
        }
    }

    Header {
        id: header
        busy: Manager.isBusy
        width: parent.width
    }

    onFlickableChanged: {
        if (flickable) {
            flickable.parent = flickableParent;
            flickable.anchors.fill = flickableParent;
        }
        scroller.flickableItem = flickable;
    }
}
