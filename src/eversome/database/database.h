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
#ifndef DATABASE_H
#define DATABASE_H

#include <QList>
#include "abstractpool.h"
#include "databaseoperation.h"

class TagItem;
class NoteItem;
class SearchItem;
class ResourceItem;
class NotebookItem;

class Database : public AbstractPool
{
    Q_OBJECT

public:
    explicit Database(QObject* parent = 0);
    virtual ~Database();

    void load(QObject* parent = 0);

    Q_INVOKABLE void saveNotebook(NotebookItem* notebook);
    Q_INVOKABLE void saveResource(ResourceItem* resource);
    Q_INVOKABLE void saveSearch(SearchItem* search);
    Q_INVOKABLE void saveNote(NoteItem* note);
    Q_INVOKABLE void saveTag(TagItem* tag);
    void save(const QList<NotebookItem*>& notebooks,
              const QList<ResourceItem*>& resources,
              const QList<SearchItem*>& searches,
              const QList<NoteItem*>& notes,
              const QList<TagItem*>& tags);

    Q_INVOKABLE void removeNotebook(NotebookItem* notebook);
    Q_INVOKABLE void removeSearch(SearchItem* search);
    Q_INVOKABLE void removeNote(NoteItem* note);
    Q_INVOKABLE void removeTag(TagItem* tag);
    void remove(const QList<NotebookItem*>& notebooks,
                const QList<SearchItem*>& searches,
                const QList<NoteItem*>& notes,
                const QList<TagItem*>& tags);

public slots:
    void open();
    void close();
    void reset();

signals:
    void loaded(const QList<NotebookItem*>& notebooks,
                const QList<ResourceItem*>& resources,
                const QList<SearchItem*>& searches,
                const QList<NoteItem*>& notes,
                const QList<TagItem*>& tags);
};

#endif // DATABASE_H
