import QtQuick 1.0
import "../../constants.js" as Constants
import com.nokia.meego 1.0
Item {
    signal itemClicked();
    signal longItemClicked();
    height: Constants.LIST_VIEW_ITEM_HEIGHT
    MouseArea{
        id: mouseArea
        anchors.fill: parent
        onClicked: {
            list.currentIndex = index;
            itemClicked();
        }
        onPressAndHold: {
            list.currentIndex = index;
            longItemClicked();
        }
    }
}

