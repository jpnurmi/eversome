#include <QtDeclarative>
#include "qmlapplicationviewer.h"

#include "synchronizer.h"
#include "userstore.h"
#include "settings.h"
#include "session.h"

#include "notebookmodel.h"
#include "notebookitem.h"
#include "resourceitem.h"
#include "noteitem.h"
#include "tagmodel.h"
#include "tagitem.h"

Q_DECL_EXPORT int main(int argc, char *argv[])
{
    QApplication::setApplicationName("EverN9");
    QApplication::setOrganizationName("Evernote");
    QScopedPointer<QApplication> app(createApplication(argc, argv));

    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));

    qmlRegisterType<TagItem>("com.evernote.types", 1,0, "Tag");
    qmlRegisterType<NoteItem>("com.evernote.types", 1,0, "Note");
    qmlRegisterType<NoteModel>("com.evernote.types", 1,0, "NoteModel");
    qmlRegisterType<ResourceItem>("com.evernote.types", 1,0, "Resource");
    qmlRegisterType<NotebookItem>("com.evernote.types", 1,0, "Notebook");

    Session session;
    QmlApplicationViewer viewer;
    viewer.setOrientation(QmlApplicationViewer::ScreenOrientationAuto);
    viewer.rootContext()->setContextProperty("UserStore", session.userStore());
    viewer.rootContext()->setContextProperty("Sync", session.synchronizer());
    viewer.rootContext()->setContextProperty("Tags", TagModel::instance());
    viewer.rootContext()->setContextProperty("Notebooks", NotebookModel::instance());

    QString username = Settings::value(Settings::Username);
    QString password = Settings::value(Settings::Password);
    if (!username.isEmpty() && !password.isEmpty()) {
        viewer.setMainQmlFile(QLatin1String("qml/EverN9/main.qml"));
        session.userStore()->auth(username, password);
    } else {
        viewer.setMainQmlFile(QLatin1String("qml/EverN9/mainLogin.qml"));
    }

    viewer.showExpanded();
    return app->exec();
}
