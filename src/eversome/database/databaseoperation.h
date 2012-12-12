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
#ifndef DATABASEOPERATION_H
#define DATABASEOPERATION_H

#include "abstractoperation.h"
#include <QSqlDatabase>
#include <QList>

class TagItem;
class NoteItem;
class SearchItem;
class ResourceItem;
class NotebookItem;

class DatabaseOperation : public AbstractOperation
{
    Q_OBJECT
    Q_ENUMS(mode)

public:
    enum Mode
    {
        OpenDatabase,
        CloseDatabase,
        ResetDatabase,
        LoadDatabase,
        SaveDatabase,
        RemoveDatabase
    };

    DatabaseOperation(Mode mode);
    ~DatabaseOperation();

    Mode mode() const;
    bool isValid() const;

    void setData(const QList<NotebookItem*>& notebooks,
                 const QList<ResourceItem*>& resources,
                 const QList<SearchItem*>& searches,
                 const QList<NoteItem*>& notes,
                 const QList<TagItem*>& tags);

signals:
    void loaded(const QList<NotebookItem*>& notebooks,
                const QList<ResourceItem*>& resources,
                const QList<SearchItem*>& searches,
                const QList<NoteItem*>& notes,
                const QList<TagItem*>& tags);

protected:
    void operate();

private:
    QList<NotebookItem*> loadNotebooks(QSqlDatabase db);
    bool saveNotebooks(const QList<NotebookItem*>& notebooks, QSqlDatabase db);
    bool removeNotebooks(const QList<NotebookItem*>& notebooks, QSqlDatabase db);

    QList<ResourceItem*> loadResources(QSqlDatabase db);
    bool saveResources(const QList<ResourceItem*>& resources, QSqlDatabase db);
    bool removeResources(const QList<ResourceItem*>& resources, QSqlDatabase db);

    QList<SearchItem*> loadSearches(QSqlDatabase db);
    bool saveSearches(const QList<SearchItem*>& searches, QSqlDatabase db);
    bool removeSearches(const QList<SearchItem*>& searches, QSqlDatabase db);

    QList<NoteItem*> loadNotes(QSqlDatabase db);
    bool saveNotes(const QList<NoteItem*>& notes, QSqlDatabase db);
    bool removeNotes(const QList<NoteItem*>& notes, QSqlDatabase db);

    QList<TagItem*> loadTags(QSqlDatabase db);
    bool saveTags(const QList<TagItem*>& tags, QSqlDatabase db);
    bool removeTags(const QList<TagItem*>& tags, QSqlDatabase db);

    QList<NotebookItem*> m_notebooks;
    QList<ResourceItem*> m_resources;
    QList<SearchItem*> m_searches;
    QList<NoteItem*> m_notes;
    QList<TagItem*> m_tags;
    Mode m_mode;
};

#endif // DATABASEOPERATION_H
