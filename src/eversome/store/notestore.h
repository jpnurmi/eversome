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

#include "abstractstore.h"
#include <Types_types.h>
#include <NoteStore_types.h>

class Session;

class NoteStore : public AbstractStore
{
    Q_OBJECT

public:
    explicit NoteStore(Session* session);
    virtual ~NoteStore();

public slots:
    void search(const evernote::edam::SavedSearch& search);

    void create(const evernote::edam::Note& note);
    void fetch(const evernote::edam::Note& note);
    void move(const evernote::edam::Note& note, const evernote::edam::Notebook& notebook);
    void rename(const evernote::edam::Note& note);
    void trash(const evernote::edam::Note& note);

signals:
    void created(const evernote::edam::Note& note);
    void fetched(const evernote::edam::Note& note);
    void moved(const evernote::edam::Note& note);
    void renamed(const evernote::edam::Note& note);
    void trashed(const evernote::edam::Note& note);

    void resourceFetched(const evernote::edam::Resource& resource);

    void searched(const evernote::edam::SavedSearch& search, const QVector<evernote::edam::NoteMetadata>& notes);
};

#endif // NOTESTORE_H
