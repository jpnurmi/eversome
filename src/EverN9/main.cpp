#include <QtDeclarative>
#include "qmlapplicationviewer.h"

#include "notestore.h"
#include "userstore.h"
#include "database.h"
#include "manager.h"

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
    qmlRegisterType<ItemModel>("com.evernote.types", 1,0, "ItemModel");
    qmlRegisterType<ResourceItem>("com.evernote.types", 1,0, "Resource");
    qmlRegisterType<NotebookItem>("com.evernote.types", 1,0, "Notebook");

    QmlApplicationViewer viewer;
    viewer.rootContext()->setContextProperty("Database", Manager::instance()->database());
    viewer.rootContext()->setContextProperty("UserStore", Manager::instance()->userStore());
    viewer.rootContext()->setContextProperty("NoteStore", Manager::instance()->noteStore());
    viewer.rootContext()->setContextProperty("Notebooks", Manager::instance()->notebookModel());
    viewer.rootContext()->setContextProperty("Tags", Manager::instance()->tagModel());
    viewer.setOrientation(QmlApplicationViewer::ScreenOrientationAuto);
    viewer.setMainQmlFile(QLatin1String("qml/EverN9/main.qml"));

    viewer.showExpanded();
    return app->exec();
}
