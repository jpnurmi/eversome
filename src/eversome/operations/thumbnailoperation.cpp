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

#include "thumbnailoperation.h"

ThumbnailOperation::ThumbnailOperation(const evernote::edam::Note& note) :
    NetworkOperation(Operation::FetchNoteThumbnail), m_note(note)
{
}

ThumbnailOperation::ThumbnailOperation(const evernote::edam::Resource& resource) :
    NetworkOperation(Operation::FetchResourceThumbnail), m_resource(resource)
{
}

ThumbnailOperation::~ThumbnailOperation()
{
}

bool ThumbnailOperation::isValid() const
{
    return NetworkOperation::isValid(); // TODO
}

void ThumbnailOperation::operate()
{
    // TODO:
    // - https://host.evernote.com/shard/shardId/thm/note/GUID.png
    // - https://host.evernote.com/shard/shardId/thm/res/GUID.png

    qDebug() << url() << authToken();

    /*
    if (operation)
        qDebug() << "DatabaseOperation::operate():" << operation
                 << "NB:" << m_notebooks.count()
                 << "R:" << m_resources.count()
                 << "S:" << m_searches.count()
                 << "N:" << m_notes.count()
                 << "T:" << m_tags.count();
    */
}
