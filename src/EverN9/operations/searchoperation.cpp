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

SearchOperation::SearchOperation(const edam::SavedSearch& search) :
    NetworkOperation(Search), m_search(search)
{
}

SearchOperation::~SearchOperation()
{
}

void SearchOperation::operate(shared_ptr<thrift::protocol::TProtocol> protocol)
{
    Q_ASSERT(mode() == Search);

    edam::NoteList list;
    edam::NoteFilter filter;
    filter.words = m_search.query;

    edam::NoteStoreClient client(protocol);
    std::string token = authToken().toStdString();
    client.findNotes(list, token, filter, 0, limits::g_Limits_constants.EDAM_USER_NOTES_MAX);

    qDebug() << "SearchOperation::operate(): searched..." << list.notes.size();

    emit searched(m_search, QVector<edam::Note>::fromStdVector(list.notes));
}
