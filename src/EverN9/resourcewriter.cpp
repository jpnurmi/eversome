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

#include "resourcewriter.h"
#include <QtConcurrentRun>
#include <QReadWriteLock>
#include <QWriteLocker>
#include <QReadLocker>
#include <QFileInfo>
#include <QVariant>
#include <QFile>
#include <QDir>

QReadWriteLock filesLock;

ResourceWriter::ResourceWriter(QObject* parent) : QObject(parent)
{
}

ResourceWriter::~ResourceWriter()
{
}

bool ResourceWriter::isWriting() const
{
    QReadLocker locker(&filesLock);
    return !m_files.isEmpty();
}

void ResourceWriter::write(const QString& filePath, const QByteArray& data)
{
    QWriteLocker locker(&filesLock);
    if (!m_files.contains(filePath)) {
        m_files.insert(filePath);
        locker.unlock();
        emit isWritingChanged();
        qDebug() << Q_FUNC_INFO << filePath << data.length();
        QtConcurrent::run(this, &ResourceWriter::writeImpl, filePath, data);
    }
}

void ResourceWriter::writeImpl(const QString& filePath, const QByteArray& data)
{
    qDebug() << Q_FUNC_INFO << filePath << data.length();

    bool succeed = false;
    QFileInfo info(filePath);
    if (QDir().mkpath(info.absolutePath())) {
        QFile file(info.filePath());
        if (file.exists())
            file.remove();
        if (file.open(QFile::WriteOnly))
            succeed = file.write(data) > 0;
        if (file.error() != QFile::NoError) {
            qDebug() << Q_FUNC_INFO << filePath << file.error();
            emit error(file.errorString());
        }
    }

    if (succeed) {
        QString thumbnail = info.baseName().replace('.', '-') + "-thumb.png";
        QStringList args = QStringList() << "-geometry" << "128x128" << info.fileName()+"[0]" << thumbnail;
        qDebug() << Q_FUNC_INFO << "/usr/bin/convert" << args;

        QProcess process;
        process.setWorkingDirectory(info.absolutePath());
        process.start("/usr/bin/convert", args);
        if (!process.waitForFinished())
            emit error(process.errorString());
        else
            emit written(info.filePath());
    }

    QWriteLocker locker(&filesLock);
    m_files.remove(filePath);
    locker.unlock();
    emit isWritingChanged();
}
