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
#ifndef NOTEBOOKPOOL_H
#define NOTEBOOKPOOL_H

#include "networkpool.h"
#include <Types_types.h>

class Session;

class NotebookPool : public NetworkPool
{
    Q_OBJECT

public:
    explicit NotebookPool(Session* session);
    virtual ~NotebookPool();

public slots:
    void create(const evernote::edam::Notebook& notebook);
    void fetch(const evernote::edam::Notebook& notebook);
    void rename(const evernote::edam::Notebook& notebook);

signals:
    void created(const evernote::edam::Notebook& notebook);
    void fetched(const evernote::edam::Notebook& notebook);
    void renamed(const evernote::edam::Notebook& notebook);
};

#endif // NOTEBOOKPOOL_H
