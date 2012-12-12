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

Database::Database(QObject* parent) : AbstractPool(parent)
{
}

Database::~Database()
{
}

void Database::open()
{
    DatabaseOperation* operation = new DatabaseOperation(DatabaseOperation::OpenDatabase);
    qDebug() << "Database::open():" << operation;
    startOperation(operation);
}

void Database::close()
{
    DatabaseOperation* operation = new DatabaseOperation(DatabaseOperation::CloseDatabase);
    qDebug() << "Database::close():" << operation;
    startOperation(operation);
}

void Database::reset()
{
    DatabaseOperation* operation = new DatabaseOperation(DatabaseOperation::ResetDatabase);
    qDebug() << "Database::reset():" << operation;
    startOperation(operation);
}

void Database::load(QObject* parent)
{
    DatabaseOperation* operation = new DatabaseOperation(DatabaseOperation::LoadDatabase);
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
    startOperation(operation);
}

void Database::saveNotebook(NotebookItem* notebook)
{
    save(QList<NotebookItem*>() << notebook,
         QList<ResourceItem*>(),
         QList<SearchItem*>(),
         QList<NoteItem*>(),
         QList<TagItem*>());
}

void Database::saveResource(ResourceItem* resource)
{
    save(QList<NotebookItem*>(),
         QList<ResourceItem*>() << resource,
         QList<SearchItem*>(),
         QList<NoteItem*>(),
         QList<TagItem*>());
}

void Database::saveSearch(SearchItem* search)
{
    save(QList<NotebookItem*>(),
         QList<ResourceItem*>(),
         QList<SearchItem*>() << search,
         QList<NoteItem*>(),
         QList<TagItem*>());
}

void Database::saveNote(NoteItem* note)
{
    save(QList<NotebookItem*>(),
         QList<ResourceItem*>(),
         QList<SearchItem*>(),
         QList<NoteItem*>() << note,
         QList<TagItem*>());
}

void Database::saveTag(TagItem* tag)
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
    DatabaseOperation* operation = new DatabaseOperation(DatabaseOperation::SaveDatabase);
    operation->setData(notebooks, resources, searches, notes, tags);
    qDebug() << "Database::save():" << operation;
    startOperation(operation);
}

void Database::removeNotebook(NotebookItem* notebook)
{
    remove(QList<NotebookItem*>() << notebook,
           QList<SearchItem*>(),
           QList<NoteItem*>(),
           QList<TagItem*>());
}

void Database::removeSearch(SearchItem* search)
{
    remove(QList<NotebookItem*>(),
           QList<SearchItem*>() << search,
           QList<NoteItem*>(),
           QList<TagItem*>());
}

void Database::removeNote(NoteItem* note)
{
    remove(QList<NotebookItem*>(),
           QList<SearchItem*>(),
           QList<NoteItem*>() << note,
           QList<TagItem*>());
}

void Database::removeTag(TagItem* tag)
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
    DatabaseOperation* operation = new DatabaseOperation(DatabaseOperation::RemoveDatabase);
    operation->setData(notebooks, QList<ResourceItem*>(), searches, notes, tags);
    qDebug() << "Database::remove():" << operation;
    startOperation(operation);
}
