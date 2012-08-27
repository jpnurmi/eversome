import QtQuick 1.1
import com.nokia.meego 1.0

import com.nokia.extras 1.0
import QtMobility.sensors 1.1



Page {
    id: loginPage

    Rectangle {
        anchors.fill: parent
        z: 0
        gradient: Gradient {
            GradientStop { position: 0.0; color: "green" }
            GradientStop { position: 0.5; color: "#99ee44" }
            GradientStop { position: 1.0; color: "green" }
        }

    }

    InfoBanner{
        id: networkErrorBanner
        x: 0
        y: parent.height-height
        z: 10
        text: qsTr("__basic_network_error__");
    }



    TextField{
        id: txtUsername
        width:parent.width*2/3
        text:""
        placeholderText: qsTr("__username__");
        anchors{
            top: imgLogin.bottom
            horizontalCenter: parent.horizontalCenter
            topMargin: 20
        }
    }


    TextField{
        id: txtPassword
        width:parent.width*2/3
        text:""
        placeholderText: qsTr("__password__")
        anchors{
            top: txtUsername.bottom
            horizontalCenter: parent.horizontalCenter
            topMargin: 10
        }
        echoMode: TextInput.Password
    }
    Connections{
        target: Session
        onAuthenticationSuccess:
        {
            hideProgress()
            var mainPage = Qt.resolvedUrl("MainPage.qml")
            pageStack.pop()
            pageStack.push(mainPage)
            Session.syncAsync()
        }
        onAuthenticationFailed:
        {
            hideProgress();
            networkErrorBanner.text = error;
            networkErrorBanner.show()
        }
    }

    Button{
        id: btnLogin
        width:parent.width*2/3
        anchors {
            horizontalCenter: parent.horizontalCenter
            top: txtPassword.bottom
            topMargin: 10
        }
        text: qsTr("__login__")
        onClicked: {
            showProgress();
            Session.authAsync(txtUsername.text,txtPassword.text)

        }
    }
    Text{
        id:btnForgotPassword
        anchors.top: btnLogin.bottom
        anchors.topMargin: 10
        anchors.horizontalCenter: parent.horizontalCenter
        textFormat: Text.RichText
        text:"<a href='#'>"+qsTr("__forgot_pass__")+"</a>"
        font.pixelSize: btnLogin.font.pixelSize
        MouseArea{
            anchors.fill: parent
            onClicked: {
                Qt.openUrlExternally("https://www.evernote.com/ForgotPassword.action");
            }
        }
    }
    Text{
        id:btnCreateAccount
        anchors.top: btnForgotPassword.bottom
        anchors.topMargin: 10
        anchors.horizontalCenter: parent.horizontalCenter
        textFormat: Text.RichText
        text:"<a href='#'>"+qsTr("__create_account__")+"</a>"
        font.pixelSize: btnLogin.font.pixelSize
        MouseArea{
            anchors.fill: parent
            onClicked: {
                Qt.openUrlExternally("https://www.evernote.com/mobile/Registration.action");
            }
        }
    }

    function showProgress(){
        txtPassword.enabled = false;
        txtUsername.enabled = false;
        btnLogin.enabled = false

    }

    function hideProgress(){
        txtPassword.enabled = true;
        txtUsername.enabled = true;
        btnLogin.enabled = true

    }


    Label{
        id: imgLogin
        anchors.topMargin: 60
        anchors.top: parent.top
        anchors.horizontalCenter: parent.horizontalCenter
        text: "EverN9"
        font.pixelSize: 80
        font.weight: Font.Light
        color: "white"
    }

    OrientationSensor{
        active: true

        onReadingChanged: {
            if(reading.orientation != 5){
                var showImage = (reading.orientation == 1)
                imgLogin.visible = showImage
                imgLogin.height = showImage ? 158 : 0
                imgLogin.anchors.topMargin = showImage ? 30 : 0
            }
        }
    }
}
