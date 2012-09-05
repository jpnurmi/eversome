#include <QtDeclarative>
#include "qmlapplicationviewer.h"

#include "notestore.h"
#include "userstore.h"
#include "database.h"
#include "manager.h"

#include "itemmodel.h"
#include "notebookitem.h"
#include "resourceitem.h"
#include "searchitem.h"
#include "noteitem.h"
#include "tagitem.h"

static bool removeDir(const QDir& dir)
{
    bool result = true;
    if (dir.exists()) {
        QFileInfoList entries = dir.entryInfoList(QDir::NoDotAndDotDot | QDir::System | QDir::Hidden  | QDir::AllDirs | QDir::Files);
        foreach (const QFileInfo &info, entries) {
            if (info.isDir())
                result &= removeDir(QDir(info.absoluteFilePath()));
            else
                result &= QFile::remove(info.absoluteFilePath());
        }
        QDir parent(dir);
        if (parent.cdUp())
            parent.rmdir(dir.dirName());
    }
    return result;
}

Q_DECL_EXPORT int main(int argc, char *argv[])
{
    QApplication::setApplicationName("EverN9");
    QApplication::setOrganizationName("Evernote");
    QScopedPointer<QApplication> app(createApplication(argc, argv));

    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));

    QString dataPath = QDesktopServices::storageLocation(QDesktopServices::DataLocation);
    QString cachePath = QDesktopServices::storageLocation(QDesktopServices::CacheLocation);
    QString configPath = QFileInfo(QSettings().fileName()).absolutePath();

    if (app->arguments().contains("-reset")) {
        qDebug() << "EverN9 reset...";
        qDebug() << "  -> Data:" << (removeDir(dataPath) ? "OK" : "FAIL!") << qPrintable("("+dataPath+")");
        qDebug() << "  -> Cache:" << (removeDir(cachePath) ? "OK" : "FAIL!") << qPrintable("("+cachePath+")");
        qDebug() << "  -> Config:" << (removeDir(configPath) ? "OK" : "FAIL!") << qPrintable("("+configPath+")");
    }

    QDir().mkpath(configPath);
    QDir().mkpath(cachePath);
    QDir().mkpath(dataPath);

    qmlRegisterType<TagItem>("com.evernote.types", 1,0, "Tag");
    qmlRegisterType<NoteItem>("com.evernote.types", 1,0, "Note");
    qmlRegisterType<ItemModel>("com.evernote.types", 1,0, "ItemModel");
    qmlRegisterType<SearchItem>("com.evernote.types", 1,0, "Search");
    qmlRegisterType<ResourceItem>("com.evernote.types", 1,0, "Resource");
    qmlRegisterType<NotebookItem>("com.evernote.types", 1,0, "Notebook");

    QmlApplicationViewer viewer;
    viewer.rootContext()->setContextProperty("Database", Manager::instance()->database());
    viewer.rootContext()->setContextProperty("UserStore", Manager::instance()->userStore());
    viewer.rootContext()->setContextProperty("NoteStore", Manager::instance()->noteStore());
    viewer.rootContext()->setContextProperty("Notebooks", Manager::instance()->notebookModel());
    viewer.rootContext()->setContextProperty("Searches", Manager::instance()->searchModel());
    viewer.rootContext()->setContextProperty("Tags", Manager::instance()->tagModel());
    viewer.setOrientation(QmlApplicationViewer::ScreenOrientationAuto);
    viewer.setMainQmlFile(QLatin1String("qml/EverN9/main.qml"));

    viewer.showExpanded();
    return app->exec();
}
