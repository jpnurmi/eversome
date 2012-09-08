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
#include <Types_types.h>
#include "operation.h"

class Session;
class NetworkOperation;

class NoteStore : public QObject
{
    Q_OBJECT

public:
    explicit NoteStore(Session* session);
    virtual ~NoteStore();

public slots:
    void sync();
    void cancel();
    void search(const evernote::edam::SavedSearch& search);

    void createNote(const evernote::edam::Note& note);
    void deleteNote(const evernote::edam::Note& note);
    void getNote(const evernote::edam::Note& note);
    void expungeNote(const evernote::edam::Note& note);
    void shareNote(const evernote::edam::Note& note);
    void unshareNote(const evernote::edam::Note& note);
    void updateNote(const evernote::edam::Note& note);

    void createNotebook(const evernote::edam::Notebook& notebook);
    void expungeNotebook(const evernote::edam::Notebook& notebook);
    void getDefaultNotebook(const evernote::edam::Notebook& notebook);
    void updateNotebook(const evernote::edam::Notebook& notebook);

signals:
    void started();
    void progress(int percent);
    void error(const QString& error);
    void finished();
    void activityChanged();

    void synced(const QVector<evernote::edam::Notebook>& notebooks,
                const QVector<evernote::edam::Resource>& resources,
                const QVector<evernote::edam::SavedSearch>& searches,
                const QVector<evernote::edam::Note>& notes,
                const QVector<evernote::edam::Tag>& tags);

    void resourceFetched(const evernote::edam::Resource& resource);
    void noteFetched(const evernote::edam::Note& note);

    void searched(const evernote::edam::SavedSearch& search, const QVector<evernote::edam::Note>& notes);

private slots:
    void onOperationStarted(Operation* operation);
    void onOperationFinished(Operation* operation);
    void onOperationError(Operation* operation, const QString& error);

private:
    void startNoteOperation(const evernote::edam::Note& note, Operation::Mode mode);
    void startNotebookOperation(const evernote::edam::Notebook& notebook, Operation::Mode mode);
    void setupOperation(NetworkOperation* operation) const;

    Session* session;
};

#endif // NOTESTORE_H
