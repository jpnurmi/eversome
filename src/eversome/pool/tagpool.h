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
#ifndef TAGPOOL_H
#define TAGPOOL_H

#include "abstractpool.h"
#include <Types_types.h>
#include <NoteStore_types.h>

class Session;

class TagPool : public AbstractPool
{
    Q_OBJECT

public:
    explicit TagPool(Session* session);
    virtual ~TagPool();

public slots:
    void create(const evernote::edam::Tag& tag);
    void fetch(const evernote::edam::Tag& tag);
    void rename(const evernote::edam::Tag& tag);

signals:
    void created(const evernote::edam::Tag& tag);
    void fetched(const evernote::edam::Tag& tag);
    void renamed(const evernote::edam::Tag& tag);
};

#endif // TAGPOOL_H
