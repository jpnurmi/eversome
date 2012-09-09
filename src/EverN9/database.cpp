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
    qDebug() << "Database::open():" << operation;
    QThreadPool::globalInstance()->start(operation);
}

void Database::close()
{
    DatabaseOperation* operation = createOperation(Operation::CloseDatabase);
    qDebug() << "Database::close():" << operation;
    QThreadPool::globalInstance()->start(operation);
}

void Database::reset()
{
    DatabaseOperation* operation = createOperation(Operation::ResetDatabase);
    qDebug() << "Database::reset():" << operation;
    QThreadPool::globalInstance()->start(operation);
}

void Database::load(QObject* parent)
{
    DatabaseOperation* operation = createOperation(Operation::LoadDatabase);
    connect(operation, SIGNAL(loaded(QList<NotebookItem*>,
                                     QList<ResourceItem*>,
                                     QList<SearchItem*>,
                                     QList<NoteItem*>,
                                     QList<TagItem*>)),
                 this, SIGNAL(loaded(QList<NotebookItem*>,
                                     QList<ResourceItem*>,
                                     QList<SearchItem*>,
                                     QList<NoteItem*>,
                                     QList<TagItem*>)));
    qDebug() << "Database::load():" << operation;
    QThreadPool::globalInstance()->start(operation);
}

void Database::save(NotebookItem* notebook)
{
    save(QList<NotebookItem*>() << notebook,
         QList<ResourceItem*>(),
         QList<SearchItem*>(),
         QList<NoteItem*>(),
         QList<TagItem*>());
}

void Database::save(ResourceItem* resource)
{
    save(QList<NotebookItem*>(),
         QList<ResourceItem*>() << resource,
         QList<SearchItem*>(),
         QList<NoteItem*>(),
         QList<TagItem*>());
}

void Database::save(SearchItem* search)
{
    save(QList<NotebookItem*>(),
         QList<ResourceItem*>(),
         QList<SearchItem*>() << search,
         QList<NoteItem*>(),
         QList<TagItem*>());
}

void Database::save(NoteItem* note)
{
    save(QList<NotebookItem*>(),
         QList<ResourceItem*>(),
         QList<SearchItem*>(),
         QList<NoteItem*>() << note,
         QList<TagItem*>());
}

void Database::save(TagItem* tag)
{
    save(QList<NotebookItem*>(),
         QList<ResourceItem*>(),
         QList<SearchItem*>(),
         QList<NoteItem*>(),
         QList<TagItem*>() << tag);
}

void Database::save(const QList<NotebookItem*>& notebooks,
                    const QList<ResourceItem*>& resources,
                    const QList<SearchItem*>& searches,
                    const QList<NoteItem*>& notes,
                    const QList<TagItem*>& tags)
{
    DatabaseOperation* operation = createOperation(Operation::SaveDatabase);
    operation->setData(notebooks, resources, searches, notes, tags);
    qDebug() << "Database::save():" << operation;
    QThreadPool::globalInstance()->start(operation);
}

void Database::remove(NotebookItem* notebook)
{
    remove(QList<NotebookItem*>() << notebook,
           QList<SearchItem*>(),
           QList<NoteItem*>(),
           QList<TagItem*>());
}

void Database::remove(SearchItem* search)
{
    remove(QList<NotebookItem*>(),
           QList<SearchItem*>() << search,
           QList<NoteItem*>(),
           QList<TagItem*>());
}

void Database::remove(NoteItem* note)
{
    remove(QList<NotebookItem*>(),
           QList<SearchItem*>(),
           QList<NoteItem*>() << note,
           QList<TagItem*>());
}

void Database::remove(TagItem* tag)
{
    remove(QList<NotebookItem*>(),
           QList<SearchItem*>(),
           QList<NoteItem*>(),
           QList<TagItem*>() << tag);
}

void Database::remove(const QList<NotebookItem*>& notebooks,
                      const QList<SearchItem*>& searches,
                      const QList<NoteItem*>& notes,
                      const QList<TagItem*>& tags)
{
    DatabaseOperation* operation = createOperation(Operation::RemoveDatabase);
    operation->setData(notebooks, QList<ResourceItem*>(), searches, notes, tags);
    qDebug() << "Database::remove():" << operation;
    QThreadPool::globalInstance()->start(operation);
}

DatabaseOperation* Database::createOperation(Operation::Mode mode)
{
    DatabaseOperation* operation = new DatabaseOperation(mode);
    connect(operation, SIGNAL(started()), this, SIGNAL(activityChanged()));
    connect(operation, SIGNAL(finished()), this, SIGNAL(activityChanged()));
    connect(operation, SIGNAL(error(QString)), this, SIGNAL(error(QString)));
    return operation;
}
