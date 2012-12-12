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

#include <QHash>
#include <QObject>
#include <Types_types.h>
#include <NoteStore_types.h>

class Session;
class Database;
class FileSystem;
class NetworkPool;

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

    Database* database() const;
    NetworkPool* networkPool() const;

    enum Item { Notebook, Resource, Search, Note, Tag };
    ItemModel* itemModel(Item item) const;

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
    void onThumbnailFetched(const QString& guid, const QByteArray& data);

    void onSearchCreated(const evernote::edam::SavedSearch& search);
    void onSearchFetched(const evernote::edam::SavedSearch& search);
    void onSearchRenamed(const evernote::edam::SavedSearch& search);
    void onSearched(const evernote::edam::SavedSearch& search,
                    const QVector<evernote::edam::NoteMetadata>& notes);

private:
    void addNotes(const QList<NoteItem*>& notes);
    void removeNotes(const QList<NoteItem*>& notes);

    FileSystem* m_files;
    Database* m_database;
    NetworkPool* m_networkpool;
    QHash<Item, ItemModel*> m_itemmodels;
};

#endif // MANAGER_H
