import QtQuick 1.1
import com.nokia.meego 1.1
import com.nokia.extras 1.0
import "UIConstants.js" as UI

PageStackWindow {
    id: window

    Component {
        id: mainPage
        MainPage {
            onLoggedOut: {
                UserStore.logout();
                loginSheet.open();
                window.pageStack.replace(mainPage, {}, true);
            }

            InfoBanner {
                id: banner

                Connections {
                    target: NoteStore
                    onError: {
                        banner.text = error;
                        banner.show();
                    }
                }
            }
        }
    }

    LoginSheet {
        id: loginSheet
        onAccepted: UserStore.login(username, password)

        Connections {
            target: UserStore
            onError: {
                loginSheet.error = error;
                loginSheet.open();
            }
            onLoggedIn: loginSheet.error = ""
        }
    }

    Component.onCompleted: {
        theme.colorScheme = 2
        pageStack.push(mainPage);
        if (UserStore.hasCredentials())
            UserStore.login();
        else
            loginSheet.open();
    }
}
