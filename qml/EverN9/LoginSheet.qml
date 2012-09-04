import QtQuick 1.1
import com.nokia.meego 1.1
import "UIConstants.js" as UI

Sheet {
    id: sheet

    property bool acceptable: username.text && password.text
    property alias username: username.text
    property alias password: password.text
    property alias error: errorLabel.text

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

            Text {
                id: errorLabel
                color: "red"
                font.pixelSize: UI.MEDIUM_FONT
            }

            Text {
                id: accountLabel
                text: qsTr("<a href='https://www.evernote.com/mobile/Registration.action'>Create an account</a>")
                font.pixelSize: UI.SMALL_FONT
                anchors.right: parent.right
                onLinkActivated: Qt.openUrlExternally(link)
            }

            TextField {
                id: username
                width: parent.width
                placeholderText: qsTr("Username");
                inputMethodHints: Qt.ImhNoAutoUppercase | Qt.ImhNoPredictiveText
            }

            TextField{
                id: password
                width: parent.width
                placeholderText: qsTr("Password")
                echoMode: TextInput.Password
            }

            Text {
                id: passwordLabel
                text: qsTr("<a href='https://www.evernote.com/ForgotPassword.action'>Forgot your password?</a>")
                font.pixelSize: UI.SMALL_FONT
                anchors.right: parent.right
                onLinkActivated: Qt.openUrlExternally(link)
            }
        }
    }
}
