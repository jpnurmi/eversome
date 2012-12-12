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

#include "tagoperation.h"
#include <NoteStore.h>

using namespace boost;
using namespace apache;
using namespace evernote;

TagOperation::TagOperation(const edam::Tag& tag, Mode mode) :
    NetworkOperation(mode), m_tag(tag)
{
}

TagOperation::~TagOperation()
{
}

void TagOperation::operate(shared_ptr<thrift::protocol::TProtocol> protocol)
{
    edam::NoteStoreClient client(protocol);
    std::string token = authToken().toStdString();
    switch (mode())
    {
        case CreateTag:
            client.createTag(m_tag, token, m_tag);
            emit created(m_tag);
            break;
        case FetchTag:
            client.getTag(m_tag, token, m_tag.guid);
            emit fetched(m_tag);
            break;
        case RenameTag:
            client.updateTag(token, m_tag);
            emit renamed(m_tag);
            break;
        default:
            Q_ASSERT(false);
            break;
    }
}
