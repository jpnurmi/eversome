#include <QtDeclarative>
#include "qmlapplicationviewer.h"
#include "evernotesession.h"
#include "cache.h"
#include "wrappers/tagwrapper.h"
#include "wrappers/notewrapper.h"
#include "wrappers/resourcewrapper.h"
#include "wrappers/notebookwrapper.h"

Q_DECL_EXPORT int main(int argc, char *argv[])
{
    QApplication::setApplicationName("EverN9");
    QScopedPointer<QApplication> app(createApplication(argc, argv));

    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));

    QTranslator translator;
    if (translator.load("EverN9." + QLocale::system().name(), ":/"))
        app->installTranslator(&translator);
    else if (translator.load("EverN9.en", ":/"))
        app->installTranslator(&translator);

    qmlRegisterType<TagWrapper>("com.evernote.types",1,0, "Tag");
    qmlRegisterType<NoteWrapper>("com.evernote.types",1,0, "Note");
    qmlRegisterType<ResourceWrapper>("com.evernote.types",1,0, "Resource");
    qmlRegisterType<NotebookWrapper>("com.evernote.types",1,0, "Notebook");

    QmlApplicationViewer viewer;
    viewer.rootContext()->setContextProperty("EvernoteSession", EvernoteSession::instance());
    viewer.rootContext()->setContextProperty("Cache", Cache::instance());

    viewer.setOrientation(QmlApplicationViewer::ScreenOrientationAuto);
    if (!Settings::value(Settings::Username).isEmpty() && !Settings::value(Settings::Password).isEmpty()) {
        viewer.setMainQmlFile(QLatin1String("qml/EverN9/main.qml"));
        EvernoteSession::instance()->syncAsync();
    } else {
        viewer.setMainQmlFile(QLatin1String("qml/EverN9/mainLogin.qml"));
    }
    viewer.showExpanded();
    return app->exec();
}
