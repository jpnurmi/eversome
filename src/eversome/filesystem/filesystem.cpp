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

FileSystem::FileSystem(QObject* parent) : AbstractPool(parent)
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

void FileSystem::read(const QString& guid, const QString& filePath)
{
    FileOperation* operation = new FileOperation(FileOperation::ReadFile, guid, filePath);
    connect(operation, SIGNAL(read(QString,QString,QByteArray)), this, SIGNAL(read(QString,QString,QByteArray)));
    qDebug() << "FileSystem::read():" << operation;
    startOperation(operation);
}

void FileSystem::write(const QString& guid, const QString& filePath, const QByteArray& data)
{
    FileOperation* operation = new FileOperation(FileOperation::WriteFile, guid, filePath, data);
    connect(operation, SIGNAL(wrote(QString,QString)), this, SIGNAL(wrote(QString,QString)));
    qDebug() << "FileSystem::write():" << operation;
    startOperation(operation);
}
