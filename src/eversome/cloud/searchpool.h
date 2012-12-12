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
#ifndef SEARCHPOOL_H
#define SEARCHPOOL_H

#include "abstractpool.h"
#include <Types_types.h>
#include <NoteStore_types.h>

class Session;

class SearchPool : public AbstractPool
{
    Q_OBJECT

public:
    explicit SearchPool(Session* session);
    virtual ~SearchPool();

public slots:
    void create(const evernote::edam::SavedSearch& search);
    void fetch(const evernote::edam::SavedSearch& search);
    void search(const evernote::edam::SavedSearch& search);
    void rename(const evernote::edam::SavedSearch& search);

signals:
    void created(const evernote::edam::SavedSearch& search);
    void fetched(const evernote::edam::SavedSearch& search);
    void searched(const evernote::edam::SavedSearch& search,
                  const QVector<evernote::edam::NoteMetadata>& notes);
    void renamed(const evernote::edam::SavedSearch& search);
};

#endif // SEARCHPOOL_H
