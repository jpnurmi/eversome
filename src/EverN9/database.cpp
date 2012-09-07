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

#include "database.h"
#include "databaseoperation.h"
#include <QThreadPool>

Database::Database(QObject* parent) : QObject(parent)
{
}

Database::~Database()
{
}

void Database::open()
{
    DatabaseOperation* operation = createOperation(Operation::OpenDatabase);
    qDebug() << Q_FUNC_INFO << operation;
    QThreadPool::globalInstance()->start(operation);
}

void Database::close()
{
    DatabaseOperation* operation = createOperation(Operation::CloseDatabase);
    qDebug() << Q_FUNC_INFO << operation;
    QThreadPool::globalInstance()->start(operation);
}

void Database::reset()
{
    DatabaseOperation* operation = createOperation(Operation::ResetDatabase);
    qDebug() << Q_FUNC_INFO << operation;
    QThreadPool::globalInstance()->start(operation);
}

void Database::load(QObject* parent)
{
    DatabaseOperation* operation = createOperation(Operation::LoadDatabase, parent);
    qDebug() << Q_FUNC_INFO << operation;
    QThreadPool::globalInstance()->start(operation);
}

void Database::save(const QList<NotebookItem*>& notebooks,
                    const QList<ResourceItem*>& resources,
                    const QList<SearchItem*>& searches,
                    const QList<NoteItem*>& notes,
                    const QList<TagItem*>& tags)
{
    DatabaseOperation* operation = createOperation(Operation::LoadDatabase);
    operation->setNotebooks(notebooks);
    operation->setResources(resources);
    operation->setSearches(searches);
    operation->setNotes(notes);
    operation->setTags(tags);
    qDebug() << Q_FUNC_INFO << operation;
    QThreadPool::globalInstance()->start(operation);
}

void Database::onOperationStarted(Operation* operation)
{
    qDebug() << Q_FUNC_INFO << operation;
    emit activityChanged();
}

void Database::onOperationFinished(Operation* operation)
{
    qDebug() << Q_FUNC_INFO << operation;

    DatabaseOperation* db = qobject_cast<DatabaseOperation*>(operation);
    if (db && db->mode() == Operation::LoadDatabase)
        emit loaded(db->notebooks(),
                    db->resources(),
                    db->searches(),
                    db->notes(),
                    db->tags());

    operation->deleteLater();

    emit activityChanged();
}

void Database::onOperationError(Operation* operation, const QString& str)
{
    qDebug() << Q_FUNC_INFO << operation << str;
    emit error(str);
}

DatabaseOperation* Database::createOperation(Operation::Mode mode, QObject* parent)
{
    DatabaseOperation* operation = new DatabaseOperation(mode, parent);
    connect(operation, SIGNAL(started(Operation*)),
                 this, SLOT(onOperationStarted(Operation*)), Qt::DirectConnection);
    connect(operation, SIGNAL(finished(Operation*)),
                 this, SLOT(onOperationFinished(Operation*)), Qt::DirectConnection);
    connect(operation, SIGNAL(error(Operation*,QString)),
                 this, SLOT(onOperationError(Operation*,QString)), Qt::DirectConnection);
    return operation;
}
