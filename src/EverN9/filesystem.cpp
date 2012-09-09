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
//#define QT_NO_DEBUG_OUTPUT

#include "filesystem.h"
#include "fileoperation.h"
#include <QThreadPool>

FileSystem::FileSystem(QObject* parent) : QObject(parent)
{
}

FileSystem::~FileSystem()
{
}

bool FileSystem::removeDir(const QDir& dir)
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

void FileSystem::write(const QString& filePath, const QByteArray& data)
{
    FileOperation* operation = new FileOperation(Operation::WriteFile, filePath, data);
    connect(operation, SIGNAL(started()), this, SIGNAL(activityChanged()));
    connect(operation, SIGNAL(finished()), this, SIGNAL(activityChanged()));
    connect(operation, SIGNAL(finished()), operation, SLOT(deleteLater()));
    connect(operation, SIGNAL(error(QString)), this, SIGNAL(error(QString)));
    connect(operation, SIGNAL(written(QString)), this, SIGNAL(written(QString)));
    qDebug() << Q_FUNC_INFO << operation << filePath << data.length();
    QThreadPool::globalInstance()->start(operation);
}
