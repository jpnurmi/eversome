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
#include <NoteStore_types.h>

class Session;
class SyncStore;
class NoteStore;
class NotebookStore;
class SearchStore;
class TagStore;
class Database;
class FileSystem;

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

    SyncStore* syncStore() const;
    NoteStore* noteStore() const;
    NotebookStore* notebookStore() const;
    SearchStore* searchStore() const;
    TagStore* tagStore() const;
    Database* database() const;

    ItemModel* notebookModel() const;
    ItemModel* resourceModel() const;
    ItemModel* searchModel() const;
    ItemModel* noteModel() const;
    ItemModel* tagModel() const;

signals:
    void isBusyChanged();
    void error(const QString& error);

private slots:
    void onActivityChanged();

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

    void onExpunged(const QVector<std::string>& notebooks,
                    const QVector<std::string>& searches,
                    const QVector<std::string>& notes,
                    const QVector<std::string>& tags);

    void onResourceFetched(const evernote::edam::Resource& resource);

    void onNoteCreated(const evernote::edam::Note& note);
    void onNoteFetched(const evernote::edam::Note& note);
    void onNoteMoved(const evernote::edam::Note& note);
    void onNoteRenamed(const evernote::edam::Note& note);
    void onNoteTrashed(const evernote::edam::Note& note);

    void onNotebookCreated(const evernote::edam::Notebook& notebook);
    void onNotebookFetched(const evernote::edam::Notebook& notebook);
    void onNotebookRenamed(const evernote::edam::Notebook& notebook);

    void onTagCreated(const evernote::edam::Tag& tag);
    void onTagFetched(const evernote::edam::Tag& tag);
    void onTagRenamed(const evernote::edam::Tag& tag);

    void onFileWritten(const QString& guid, const QString& filePath);

    void onSearchCreated(const evernote::edam::SavedSearch& search);
    void onSearchFetched(const evernote::edam::SavedSearch& search);
    void onSearchRenamed(const evernote::edam::SavedSearch& search);
    void onSearched(const evernote::edam::SavedSearch& search,
                    const QVector<evernote::edam::NoteMetadata>& notes);

private:
    void addNotes(const QList<NoteItem*>& notes);
    void removeNotes(const QList<NoteItem*>& notes);

    SyncStore* m_syncstore;
    NoteStore* m_notestore;
    NotebookStore* m_notebookstore;
    SearchStore* m_searchstore;
    TagStore* m_tagstore;
    Database* m_database;
    FileSystem* m_files;

    ItemModel* m_notebooks;
    ItemModel* m_resources;
    ItemModel* m_searches;
    ItemModel* m_notes;
    ItemModel* m_tags;
};

#endif // MANAGER_H
