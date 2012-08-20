import QtQuick 1.1
import Qt 4.7
import com.nokia.meego 1.0

PageStackWindow {
    id: appWindow

    initialPage: loginPage

    LoginPage {
        id: loginPage
    }

}
