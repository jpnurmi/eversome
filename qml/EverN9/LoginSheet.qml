import QtQuick 1.1
import com.nokia.meego 1.1
import "UIConstants.js" as UI

Sheet {
    id: sheet

    property bool acceptable: username.text && password.text
    property alias username: username.text
    property alias password: password.text
    property alias error: errorLabel.text
    property alias remember: remember.checked

    acceptButtonText: acceptable ? qsTr("Ok") : ""

    style: SheetStyle {
        headerBackground: "images/header.png"
    }

    title: Text {
        text: qsTr("Sign in")
        anchors.left: parent.left
        anchors.verticalCenter: parent.verticalCenter
        anchors.leftMargin: UI.PAGE_MARGIN + UI.LARGE_SPACING
        font.weight: Font.Light
        font.family: UI.FONT_FAMILY
        font.pixelSize: UI.LARGE_FONT
        textFormat: Text.PlainText
        color: "white"
        elide: Text.ElideRight
    }

    content: Flickable {
        id: flickable
        anchors.fill: parent
        anchors.margins: UI.LARGE_SPACING
        contentHeight: column.height

        Column {
            id: column
            width: parent.width
            spacing: UI.LARGE_SPACING

            Image {
                id: banner
                source: "images/banner.png"
                opacity: mouseArea.pressed && mouseArea.containsMouse ? UI.DISABLED_OPACITY : 1.0
                MouseArea {
                    id: mouseArea
                    anchors.fill: parent
                    onClicked: Qt.openUrlExternally("https://www.evernote.com/mobile/Registration.action")
                }
            }

            Text {
                id: errorLabel
                color: "red"
                font.pixelSize: UI.MEDIUM_FONT
            }

            Column {
                width: parent.width
                spacing: UI.SMALL_SPACING
                Text {
                    text: qsTr("Username")
                    font.pixelSize: UI.SMALL_FONT
                }
                TextField {
                    id: username
                    width: parent.width
                    errorHighlight: !text || sheet.error
                    inputMethodHints: Qt.ImhNoAutoUppercase | Qt.ImhNoPredictiveText
                }
            }

            Column {
                width: parent.width
                spacing: UI.SMALL_SPACING
                Text {
                    text: qsTr("Password")
                    font.pixelSize: UI.SMALL_FONT
                }
                TextField{
                    id: password
                    width: parent.width
                    errorHighlight: !text || sheet.error
                    echoMode: TextInput.Password
                }
            }

            CheckBox {
                id: remember
                width: parent.width
                text: qsTr("Remember me")
            }
        }
    }

    onStatusChanged: {
        if (status == DialogStatus.Open)
            username.forceActiveFocus();
    }
}
