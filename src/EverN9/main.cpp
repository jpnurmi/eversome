/*
* Copyright (C) 2012 J-P Nurmi <jpnurmi@gmail.com>
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*/
#include <QtDeclarative>
#include <MApplication>
#include "qmlapplicationviewer.h"

#include "filesystem.h"
#include "notestore.h"
#include "account.h"
#include "session.h"
#include "manager.h"

#include "itemmodel.h"
#include "notebookitem.h"
#include "resourceitem.h"
#include "searchitem.h"
#include "noteitem.h"
#include "tagitem.h"

static const QLatin1String DEFAULT_HOST("www.evernote.com");

Q_DECL_EXPORT int main(int argc, char* argv[])
{
    QApplication::setApplicationName("EverN9");
    QApplication::setOrganizationName("Evernote");
    MApplication app(argc, argv);

    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));

    QString dataPath = QDesktopServices::storageLocation(QDesktopServices::DataLocation);
    QString cachePath = QDesktopServices::storageLocation(QDesktopServices::CacheLocation);
    QString configPath = QFileInfo(QSettings().fileName()).absolutePath();

    QStringList args = app.arguments();
    if (args.contains("-reset")) {
        qDebug() << "EverN9 reset...";
        qDebug() << "  -> Data:" << (FileSystem::removeDir(dataPath) ? "OK" : "FAIL!") << qPrintable("("+dataPath+")");
        qDebug() << "  -> Cache:" << (FileSystem::removeDir(cachePath) ? "OK" : "FAIL!") << qPrintable("("+cachePath+")");
        qDebug() << "  -> Config:" << (FileSystem::removeDir(configPath) ? "OK" : "FAIL!") << qPrintable("("+configPath+")");
    }

    QDir().mkpath(configPath);
    QDir().mkpath(cachePath);
    QDir().mkpath(dataPath);

    QString host = DEFAULT_HOST;
    int index = args.indexOf("-host");
    if (index != -1)
        host = args.value(index + 1);

    qmlRegisterType<TagItem>("com.evernote.types", 1,0, "Tag");
    qmlRegisterType<NoteItem>("com.evernote.types", 1,0, "Note");
    qmlRegisterType<ItemModel>("com.evernote.types", 1,0, "ItemModel");
    qmlRegisterType<SearchItem>("com.evernote.types", 1,0, "Search");
    qmlRegisterType<ResourceItem>("com.evernote.types", 1,0, "Resource");
    qmlRegisterType<NotebookItem>("com.evernote.types", 1,0, "Notebook");

    Account account;
    Session session(host);
    Manager* manager = new Manager(&session);
    QmlApplicationViewer viewer;
    viewer.rootContext()->setContextProperty("Session", &session);
    viewer.rootContext()->setContextProperty("Manager", manager);
    viewer.rootContext()->setContextProperty("NoteStore", manager->noteStore());
    viewer.rootContext()->setContextProperty("Notebooks", manager->notebookModel());
    viewer.rootContext()->setContextProperty("Searches", manager->searchModel());
    viewer.rootContext()->setContextProperty("Tags", manager->tagModel());
    viewer.setOrientation(QmlApplicationViewer::ScreenOrientationAuto);
    viewer.setMainQmlFile(QLatin1String("qml/EverN9/main.qml"));

    if (account.init()) {
        session.establish(account.credentialsId());
        viewer.showExpanded();
    } else {
        QObject::connect(&account, SIGNAL(failed()), &app, SLOT(quit())); // TODO
        QObject::connect(&account, SIGNAL(cancelled()), &app, SLOT(quit()));
        QObject::connect(&account, SIGNAL(created(int)), &session, SLOT(establish(int)));
        QObject::connect(&account, SIGNAL(created(int)), &viewer, SLOT(showFullScreen()));
        QTimer::singleShot(0, &account, SLOT(create()));
    }

    return app.exec();
}
