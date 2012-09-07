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
#include "fileoperation.h"
#include <QThreadPool>

ResourceWriter::ResourceWriter(QObject* parent) : QObject(parent)
{
}

ResourceWriter::~ResourceWriter()
{
}

bool ResourceWriter::isActive() const
{
    return QThreadPool::globalInstance()->activeThreadCount();
}

void ResourceWriter::write(const QString& filePath, const QByteArray& data)
{
    FileOperation* operation = new FileOperation(Operation::WriteFile, filePath, data);
    qDebug() << Q_FUNC_INFO << operation << filePath << data.length();
    QThreadPool::globalInstance()->start(operation);
}

void ResourceWriter::onOperationStarted(Operation* operation)
{
    qDebug() << Q_FUNC_INFO << operation;
    emit isActiveChanged();
}

void ResourceWriter::onOperationFinished(Operation* operation)
{
    qDebug() << Q_FUNC_INFO << operation;

    FileOperation* fileOperation = qobject_cast<FileOperation*>(operation);
    if (fileOperation && fileOperation->data().length())
        emit written(fileOperation->filePath());

    operation->deleteLater();

    emit isActiveChanged();
}

void ResourceWriter::onOperationError(Operation* operation, const QString& error)
{
    qDebug() << Q_FUNC_INFO << operation << error;
}
