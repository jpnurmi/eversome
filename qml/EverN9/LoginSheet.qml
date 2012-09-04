import QtQuick 1.1
import com.nokia.meego 1.1
import "UIConstants.js" as UI

Sheet {
    id: sheet

    property bool acceptable: username.text && password.text
    property alias username: username.text
    property alias password: password.text

    acceptButtonText: acceptable ? qsTr("Sign in") : ""
    rejectButtonText: qsTr("Cancel")

    style: SheetStyle {
        headerBackground: "images/header.png"
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
