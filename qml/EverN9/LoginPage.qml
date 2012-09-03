import QtQuick 1.1
import com.nokia.meego 1.1
import "UIConstants.js" as UI

Page {
    id: root

    property alias username: username.text
    property alias password: password.text

    signal accepted()

    BorderImage {
        anchors.fill: parent
        source: "images/login-background.png"
        border { top: 12; left: 12; right: 12; bottom: 12 }
    }

    Text {
        id: accountLabel
        text: qsTr("<a href='https://www.evernote.com/mobile/Registration.action'>Create an account</a>")
        anchors.top: parent.top
        anchors.right: parent.right
        anchors.margins: UI.LARGE_SPACING
        font.pixelSize: UI.MEDIUM_FONT
        onLinkActivated: Qt.openUrlExternally(link)
    }

    Column {
        id: column
        width: root.width * 2/3
        anchors.centerIn: parent
        spacing: UI.LARGE_SPACING

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

        Item {
            width: parent.width
            height: UI.LARGE_SPACING
        }

        Button {
            id: button
            width: accountLabel.width
            anchors.right: parent.right
            text: qsTr("Sign in")
            onClicked: root.accepted()
        }
    }

    Text {
        id: passwordLabel
        text: qsTr("<a href='https://www.evernote.com/ForgotPassword.action'>Forgot your password?</a>")
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.margins: UI.LARGE_SPACING
        font.pixelSize: UI.MEDIUM_FONT
        onLinkActivated: Qt.openUrlExternally(link)
    }
}
