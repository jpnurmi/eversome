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
#ifndef MANAGER_H
#define MANAGER_H

#include <QObject>
#include <Types_types.h>

class Session;
class NoteStore;
class Database;
class ResourceWriter;

class ItemModel;
class NotebookItem;
class ResourceItem;
class SearchItem;
class NoteItem;
class TagItem;

class Manager : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool isBusy READ isBusy NOTIFY isBusyChanged)

public:
    explicit Manager(Session* session = 0);
    virtual ~Manager();

    bool isBusy() const;

    NoteStore* noteStore() const;

    ItemModel* notebookModel() const;
    ItemModel* resourceModel() const;
    ItemModel* searchModel() const;
    ItemModel* noteModel() const;
    ItemModel* tagModel() const;

signals:
    void isBusyChanged();

private slots:
    void onLoaded(const QList<NotebookItem*>& notebooks,
                  const QList<ResourceItem*>& resources,
                  const QList<SearchItem*>& searches,
                  const QList<NoteItem*>& notes,
                  const QList<TagItem*>& tags);

    void onSynced(const QVector<evernote::edam::Notebook>& notebooks,
                  const QVector<evernote::edam::Resource>& resources,
                  const QVector<evernote::edam::SavedSearch>& searches,
                  const QVector<evernote::edam::Note>& notes,
                  const QVector<evernote::edam::Tag>& tags);

    void onResourceFetched(const evernote::edam::Resource& resource);
    void onNoteFetched(const evernote::edam::Note& note);

    void onResourceWritten(const QString& filePath);

    void onSearched(const evernote::edam::SavedSearch& search, const QVector<evernote::edam::Note>& notes);

private:
    void setupNotes(const QList<NoteItem*>& notes);

    NoteStore* m_store;
    Database* m_database;
    ResourceWriter* m_writer;

    ItemModel* m_notebooks;
    ItemModel* m_resources;
    ItemModel* m_searches;
    ItemModel* m_notes;
    ItemModel* m_tags;
};

#endif // MANAGER_H
