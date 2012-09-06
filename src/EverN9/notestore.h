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

class UserStore;
class Operation;

class NoteStore : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool isActive READ isActive NOTIFY isActiveChanged)

public:
    explicit NoteStore(UserStore *userStore);
    virtual ~NoteStore();

    bool isActive() const;

public slots:
    void sync();
    void cancel();
    void fetch(const evernote::edam::Note& note);
    void search(const evernote::edam::SavedSearch& search);

signals:
    void started();
    void progress(int percent);
    void error(const QString& error);
    void finished();
    void isActiveChanged();

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
    void setupOperation(Operation* operation) const;

    volatile bool syncing;
    volatile bool fetching;
    volatile bool searching;
    volatile bool cancelled;
    UserStore* userStore;
};

#endif // NOTESTORE_H
