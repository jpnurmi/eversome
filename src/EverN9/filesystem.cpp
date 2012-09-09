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

void FileSystem::read(const QString& filePath)
{
    FileOperation* operation = new FileOperation(Operation::ReadFile, filePath);
    connect(operation, SIGNAL(read(QString)), this, SIGNAL(readed(QString)));
    setupOperation(operation);
    qDebug() << "FileSystem::read():" << operation;
    QThreadPool::globalInstance()->start(operation);
}

void FileSystem::write(const QString& filePath, const QByteArray& data)
{
    FileOperation* operation = new FileOperation(Operation::WriteFile, filePath, data);
    connect(operation, SIGNAL(written(QString)), this, SIGNAL(written(QString)));
    setupOperation(operation);
    qDebug() << "FileSystem::write():" << operation;
    QThreadPool::globalInstance()->start(operation);
}

void FileSystem::generate(const QString& filePath)
{
    FileOperation* operation = new FileOperation(Operation::GenerateThumbnail, filePath);
    connect(operation, SIGNAL(generated(QString)), this, SIGNAL(generated(QString)));
    setupOperation(operation);
    qDebug() << "FileSystem::generateThumbnail():" << operation;
    QThreadPool::globalInstance()->start(operation);
}

void FileSystem::setupOperation(Operation *operation)
{
    connect(operation, SIGNAL(started()), this, SIGNAL(activityChanged()));
    connect(operation, SIGNAL(finished()), this, SIGNAL(activityChanged()));
    connect(operation, SIGNAL(error(QString)), this, SIGNAL(error(QString)));
}
