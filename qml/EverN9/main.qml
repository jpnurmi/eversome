import QtQuick 1.1
import com.nokia.meego 1.1
import "UIConstants.js" as UI

PageStackWindow {
    id: window

    Component {
        id: mainPage
        MainPage { }
    }

    Component {
        id: loginPage
        LoginPage {
            onAccepted: {
                UserStore.authenticate(username, password);
                pageStack.replace(mainPage);
            }
        }
    }

    Component.onCompleted: {
        theme.colorScheme = 2
        if (UserStore.hasCredentials()) {
            UserStore.authenticate();
            pageStack.push(mainPage, {}, true);
        } else {
            pageStack.push(loginPage, {}, true);
        }
    }
}
