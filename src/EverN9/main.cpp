#include <QtDeclarative>
#include "qmlapplicationviewer.h"

#include "notestore.h"
#include "userstore.h"
#include "session.h"

#include "itemmodel.h"
#include "notebookitem.h"
#include "resourceitem.h"
#include "noteitem.h"
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

    QmlApplicationViewer viewer;
    viewer.rootContext()->setContextProperty("UserStore", Session::instance()->userStore());
    viewer.rootContext()->setContextProperty("NoteStore", Session::instance()->noteStore());
    viewer.rootContext()->setContextProperty("Tags", Session::instance()->tagModel());
    viewer.rootContext()->setContextProperty("Notebooks", Session::instance()->notebookModel());
    viewer.setOrientation(QmlApplicationViewer::ScreenOrientationAuto);
    viewer.setMainQmlFile(QLatin1String("qml/EverN9/main.qml"));
    viewer.showExpanded();
    return app->exec();
}
