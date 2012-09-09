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

#include "operation.h"
#include <QList>

class TagItem;
class NoteItem;
class SearchItem;
class ResourceItem;
class NotebookItem;

class DatabaseOperation : public Operation
{
    Q_OBJECT

public:
    DatabaseOperation(Mode mode);
    ~DatabaseOperation();

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
    QList<NotebookItem*> loadNotebooks();
    void saveNotebooks(const QList<NotebookItem*>& notebooks);
    void removeNotebooks(const QList<NotebookItem*>& notebooks);

    QList<ResourceItem*> loadResources();
    void saveResources(const QList<ResourceItem*>& resources);
    void removeResources(const QList<ResourceItem*>& resources);

    QList<SearchItem*> loadSearches();
    void saveSearches(const QList<SearchItem*>& searches);
    void removeSearches(const QList<SearchItem*>& searches);

    QList<NoteItem*> loadNotes();
    void saveNotes(const QList<NoteItem*>& notes);
    void removeNotes(const QList<NoteItem*>& notes);

    QList<TagItem*> loadTags();
    void saveTags(const QList<TagItem*>& tags);
    void removeTags(const QList<TagItem*>& tags);

    QList<NotebookItem*> m_notebooks;
    QList<ResourceItem*> m_resources;
    QList<SearchItem*> m_searches;
    QList<NoteItem*> m_notes;
    QList<TagItem*> m_tags;
};

#endif // DATABASEOPERATION_H
