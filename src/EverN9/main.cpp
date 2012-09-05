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
#include "qmlapplicationviewer.h"

#include "notestore.h"
#include "userstore.h"
#include "settings.h"
#include "manager.h"

#include "itemmodel.h"
#include "notebookitem.h"
#include "resourceitem.h"
#include "searchitem.h"
#include "noteitem.h"
#include "tagitem.h"

static const QLatin1String CONSUMER_KEY("everel");
static const QLatin1String CONSUMER_SECRET("201d20eb3ee1f74d");
static const QLatin1String DEFAULT_HOST("www.evernote.com");
static const int DEFAULT_PORT = 80;

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

    QStringList args = app->arguments();
    if (args.contains("-reset")) {
        qDebug() << "EverN9 reset...";
        qDebug() << "  -> Data:" << (removeDir(dataPath) ? "OK" : "FAIL!") << qPrintable("("+dataPath+")");
        qDebug() << "  -> Cache:" << (removeDir(cachePath) ? "OK" : "FAIL!") << qPrintable("("+cachePath+")");
        qDebug() << "  -> Config:" << (removeDir(configPath) ? "OK" : "FAIL!") << qPrintable("("+configPath+")");
    }

    QDir().mkpath(configPath);
    QDir().mkpath(cachePath);
    QDir().mkpath(dataPath);

    QString key = CONSUMER_KEY;
    int index = args.indexOf("-key");
    if (index != -1)
        key = args.value(index + 1);
    Settings::setValue(Settings::ConsumerKey, key);

    QString secret = CONSUMER_SECRET;
    index = args.indexOf("-secret");
    if (index != -1)
        secret = args.value(index + 1);
    Settings::setValue(Settings::ConsumerSecret, secret);

    QString host = DEFAULT_HOST;
    index = args.indexOf("-host");
    if (index != -1)
        host = args.value(index + 1);
    Settings::setValue(Settings::Hostname, host);

    int port = DEFAULT_PORT;
    index = args.indexOf("-port");
    if (index != -1)
        port = args.value(index + 1).toInt();
    Settings::setValue(Settings::ServerPort, QString::number(port));

    qmlRegisterType<TagItem>("com.evernote.types", 1,0, "Tag");
    qmlRegisterType<NoteItem>("com.evernote.types", 1,0, "Note");
    qmlRegisterType<ItemModel>("com.evernote.types", 1,0, "ItemModel");
    qmlRegisterType<SearchItem>("com.evernote.types", 1,0, "Search");
    qmlRegisterType<ResourceItem>("com.evernote.types", 1,0, "Resource");
    qmlRegisterType<NotebookItem>("com.evernote.types", 1,0, "Notebook");

    Manager manager;
    QmlApplicationViewer viewer;
    viewer.rootContext()->setContextProperty("Manager", &manager);
    viewer.rootContext()->setContextProperty("UserStore", manager.userStore());
    viewer.rootContext()->setContextProperty("NoteStore", manager.noteStore());
    viewer.rootContext()->setContextProperty("Notebooks", manager.notebookModel());
    viewer.rootContext()->setContextProperty("Searches", manager.searchModel());
    viewer.rootContext()->setContextProperty("Tags", manager.tagModel());
    viewer.setOrientation(QmlApplicationViewer::ScreenOrientationAuto);
    viewer.setMainQmlFile(QLatin1String("qml/EverN9/main.qml"));

    viewer.showExpanded();
    return app->exec();
}
