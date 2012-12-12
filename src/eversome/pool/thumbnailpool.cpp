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
//#define QT_NO_DEBUG_OUTPUT

#include "thumbnailpool.h"
#include "thumbnailoperation.h"
#include <QMetaType>
#include <QtDebug>

using namespace evernote;

ThumbnailPool::ThumbnailPool(Session* session) : AbstractPool(session)
{
}

ThumbnailPool::~ThumbnailPool()
{
}

void ThumbnailPool::fetch(const edam::Note& note)
{
    ThumbnailOperation* operation = new ThumbnailOperation(note);
    connect(operation, SIGNAL(fetched(evernote::edam::Note)),
                 this, SIGNAL(fetched(evernote::edam::Note)));
    startOperation(operation, "thm/note");
}

void ThumbnailPool::fetch(const edam::Resource& resource)
{
    ThumbnailOperation* operation = new ThumbnailOperation(resource);
    connect(operation, SIGNAL(fetched(evernote::edam::Resource)),
                 this, SIGNAL(fetched(evernote::edam::Resource)));
    startOperation(operation, "thm/res");
}
