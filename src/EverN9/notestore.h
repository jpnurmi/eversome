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
#ifndef NOTESTORE_H
#define NOTESTORE_H

#include <QObject>
#include <QDateTime>
#include <Types_types.h>
#include <NoteStore_types.h>
#include "operation.h"

class Session;
class NetworkOperation;

class NoteStore : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int usn READ usn NOTIFY usnChanged)
    Q_PROPERTY(QDateTime currentTime READ currentTime NOTIFY currentTimeChanged)

public:
    explicit NoteStore(Session* session);
    virtual ~NoteStore();

    int usn() const;
    QDateTime currentTime() const;

public slots:
    void sync();
    void cancel();
    void search(const evernote::edam::SavedSearch& search);

    void createNote(const evernote::edam::Note& note);
    void deleteNote(const evernote::edam::Note& note);
    void getNote(const evernote::edam::Note& note);
    void shareNote(const evernote::edam::Note& note);
    void unshareNote(const evernote::edam::Note& note);
    void updateNote(const evernote::edam::Note& note);

    void createNotebook(const evernote::edam::Notebook& notebook);
    void getDefaultNotebook(const evernote::edam::Notebook& notebook);
    void updateNotebook(const evernote::edam::Notebook& notebook);

signals:
    void usnChanged();
    void activityChanged();
    void currentTimeChanged();
    void error(const QString& error);

    void synced(const QVector<evernote::edam::Notebook>& notebooks,
                const QVector<evernote::edam::Resource>& resources,
                const QVector<evernote::edam::SavedSearch>& searches,
                const QVector<evernote::edam::Note>& notes,
                const QVector<evernote::edam::Tag>& tags);

    void expunged(const QVector<std::string>& notebooks,
                  const QVector<std::string>& searches,
                  const QVector<std::string>& notes,
                  const QVector<std::string>& tags);

    void resourceFetched(const evernote::edam::Resource& resource);
    void noteFetched(const evernote::edam::Note& note);

    void searched(const evernote::edam::SavedSearch& search, const QVector<evernote::edam::Note>& notes);

private slots:
    void setUsn(int usn);
    void setCurrentTime(const QDateTime& time);

private:
    void startNoteOperation(const evernote::edam::Note& note, Operation::Mode mode);
    void startNotebookOperation(const evernote::edam::Notebook& notebook, Operation::Mode mode);
    void setupOperation(NetworkOperation* operation) const;

    Session* session;
};

#endif // NOTESTORE_H
