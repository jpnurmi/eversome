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

#include "searchoperation.h"
#include <Limits_constants.h>
#include <NoteStore.h>

using namespace boost;
using namespace apache;
using namespace evernote;

SearchOperation::SearchOperation(const edam::SavedSearch& search, Mode mode) :
    NetworkOperation(mode), m_search(search)
{
}

SearchOperation::~SearchOperation()
{
}

void SearchOperation::operate(shared_ptr<thrift::protocol::TProtocol> protocol)
{
    edam::NoteStoreClient client(protocol);
    std::string token = authToken().toStdString();
    switch (mode())
    {
        case CreateTag:
            client.createSearch(m_search, token, m_search);
            emit created(m_search);
            break;
        case FetchTag:
            client.getSearch(m_search, token, m_search.guid);
            emit fetched(m_search);
            break;
        case PerformSearch:
        {
            edam::NoteFilter filter;
            edam::NotesMetadataList list;
            edam::NotesMetadataResultSpec spec;
            filter.__isset.words = true; // :(
            filter.words = m_search.query;
            client.findNotesMetadata(list, token, filter, 0, limits::g_Limits_constants.EDAM_USER_NOTES_MAX, spec);
            emit searched(m_search, QVector<edam::NoteMetadata>::fromStdVector(list.notes));
        }
        case RenameSearch:
            client.updateSearch(token, m_search);
            emit renamed(m_search);
            break;
        default:
            Q_ASSERT(false);
            break;
    }
}
