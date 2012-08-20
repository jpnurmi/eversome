import QtQuick 1.1
import com.nokia.meego 1.0

import com.nokia.extras 1.0

Sheet{

    property alias text: txtText.text

    rejectButtonText: qsTr("__cancel__")

    content: TextEdit{
        id:txtText
        textFormat: TextEdit.RichText
        anchors.fill: parent

    }
    function clear(){
        text = "";
    }
    onAccepted: {
        console.log(txtText.text);
    }
}
