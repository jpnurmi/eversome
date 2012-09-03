import QtQuick 1.1
import com.nokia.meego 1.1
import "UIConstants.js" as UI

PageStackWindow {
    id: window

    initialPage: MainPage { }

    LoginSheet {
        id: loginSheet
        onAccepted: UserStore.authenticate(username, password)
    }

    Component.onCompleted: {
        theme.colorScheme = 2
        if (UserStore.hasCredentials())
            UserStore.authenticate();
        else
            loginSheet.open();
    }
}
