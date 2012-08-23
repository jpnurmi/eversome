#include <QtGui/QApplication>
#include "qmlapplicationviewer.h"
#include "evernotesession.h"
#include "db/databasemanager.h"
#include "cache.h"
#include "wrappers/tagwrapper.h"
#include "wrappers/resourcewrapper.h"
#include "wrappers/notebookwrapper.h"

#include <QtDeclarative>
Q_DECL_EXPORT int main(int argc, char *argv[])
{
    QScopedPointer<QApplication> app(createApplication(argc, argv));
    QScopedPointer<QmlApplicationViewer> viewer(QmlApplicationViewer::create());

    QString locale = QLocale::system().name();

    QTranslator translator;
    if (translator.load("EverN9." + locale, ":/")){
        app.data()->installTranslator(&translator);
    }else{
        translator.load("EverN9.en", ":/");
        app.data()->installTranslator(&translator);

    }


    qmlRegisterType <TagWrapper> ("com.evernote.types",1,0, "Tag");
    qmlRegisterType <NoteWrapper> ("com.evernote.types",1,0, "Note");
    qmlRegisterType <ResourceWrapper> ("com.evernote.types",1,0, "Resource");
    qmlRegisterType <NotebookWrapper> ("com.evernote.types",1,0, "Notebook");

    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));

    viewer->rootContext()->setContextProperty("EvernoteSession",EvernoteSession::instance());
    viewer->rootContext()->setContextProperty("DatabaseManager",DatabaseManager::instance());
    viewer->rootContext()->setContextProperty("Cache",Cache::instance());

    viewer->setOrientation(QmlApplicationViewer::ScreenOrientationAuto);
    if(Settings::instance()->areCredentialsSaved()){
        viewer->setMainQmlFile(QLatin1String("qml/EverN9/main.qml"));
        EvernoteSession::instance()->syncAsync();
    }else{
        viewer->setMainQmlFile(QLatin1String("qml/EverN9/mainLogin.qml"));
    }

    QObject::connect(viewer->engine(), SIGNAL(quit()), viewer.data(), SLOT(close()));
    viewer->showExpanded();



    return app->exec();
}
