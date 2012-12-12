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
#ifndef RESOURCEPOOL_H
#define RESOURCEPOOL_H

#include "networkpool.h"
#include <Types_types.h>
#include <NoteStore_types.h>

class Session;

class ResourcePool : public NetworkPool
{
    Q_OBJECT

public:
    explicit ResourcePool(Session* session);
    virtual ~ResourcePool();

public slots:
    void fetch(const evernote::edam::Resource& resource);

signals:
    void fetched(const evernote::edam::Resource& resource);
};

#endif // RESOURCEPOOL_H
