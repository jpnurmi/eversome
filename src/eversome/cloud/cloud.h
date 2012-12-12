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
#ifndef CLOUD_H
#define CLOUD_H

#include "abstractpool.h"
#include <Types_types.h>
#include <NoteStore_types.h>
#include <QDateTime>

class Session;
class NetworkOperation;

class Cloud : public AbstractPool
{
    Q_OBJECT
    Q_PROPERTY(int usn READ usn NOTIFY usnChanged)
    Q_PROPERTY(QDateTime currentTime READ currentTime NOTIFY currentTimeChanged)

public:
    explicit Cloud(Session* session);
    virtual ~Cloud();

    Session* session() const;

    int usn() const;
    QDateTime currentTime() const;

public slots:
    void sync();

    void createNotebook(const evernote::edam::Notebook& notebook);
    void fetchNotebook(const evernote::edam::Notebook& notebook);
    void renameNotebook(const evernote::edam::Notebook& notebook);

    void createNote(const evernote::edam::Note& note);
    void fetchNote(const evernote::edam::Note& note);
    void moveNote(const evernote::edam::Note& note, const evernote::edam::Notebook& notebook);
    void renameNote(const evernote::edam::Note& note);
    void trashNote(const evernote::edam::Note& note);

    void createTag(const evernote::edam::Tag& tag);
    void fetchTag(const evernote::edam::Tag& tag);
    void renameTag(const evernote::edam::Tag& tag);

    void createSearch(const evernote::edam::SavedSearch& search);
    void fetchSearch(const evernote::edam::SavedSearch& search);
    void search(const evernote::edam::SavedSearch& search);
    void renameSearch(const evernote::edam::SavedSearch& search);

    void fetchResource(const evernote::edam::Resource& resource);

    void fetchThumbnail(const evernote::edam::Note& note);
    void fetchThumbnail(const evernote::edam::Resource& resource);

signals:
    void usnChanged();
    void currentTimeChanged();

    void synced(const QVector<evernote::edam::Notebook>& notebooks,
                const QVector<evernote::edam::Resource>& resources,
                const QVector<evernote::edam::SavedSearch>& searches,
                const QVector<evernote::edam::Note>& notes,
                const QVector<evernote::edam::Tag>& tags);

    void expunged(const QVector<std::string>& notebooks,
                  const QVector<std::string>& searches,
                  const QVector<std::string>& notes,
                  const QVector<std::string>& tags);

    void notebookCreated(const evernote::edam::Notebook& notebook);
    void notebookFetched(const evernote::edam::Notebook& notebook);
    void notebookRenamed(const evernote::edam::Notebook& notebook);

    void noteCreated(const evernote::edam::Note& note);
    void noteFetched(const evernote::edam::Note& note);
    void noteMoved(const evernote::edam::Note& note);
    void noteRenamed(const evernote::edam::Note& note);
    void noteTrashed(const evernote::edam::Note& note);

    void tagCreated(const evernote::edam::Tag& tag);
    void tagFetched(const evernote::edam::Tag& tag);
    void tagRenamed(const evernote::edam::Tag& tag);

    void searchCreated(const evernote::edam::SavedSearch& search);
    void searchFetched(const evernote::edam::SavedSearch& search);
    void searched(const evernote::edam::SavedSearch& search,
                  const QVector<evernote::edam::NoteMetadata>& notes);
    void searchRenamed(const evernote::edam::SavedSearch& search);

    void resourceFetched(const evernote::edam::Resource& resource);
    void thumbnailFetched(const QString& guid, const QByteArray& data);

private slots:
    void setUsn(int usn);
    void setCurrentTime(const QDateTime& time);

private:
    using AbstractPool::startOperation;
    void startOperation(NetworkOperation* operation, const QString& path);

    Session* m_session;
};

#endif // CLOUD_H
