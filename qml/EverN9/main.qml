import QtQuick 1.1
import com.nokia.meego 1.1
import "UIConstants.js" as UI

PageStackWindow {
    id: window

    initialPage: MainPage {
        onLoggedOut: {
            UserStore.logout();
            loginSheet.open();
        }
    }

    LoginSheet {
        id: loginSheet
        onAccepted: UserStore.login(username, password)
    }

    Component.onCompleted: {
        theme.colorScheme = 2
        if (UserStore.hasCredentials())
            UserStore.login();
        else
            loginSheet.open();
    }
}
