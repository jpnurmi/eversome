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

#include "resourceoperation.h"
#include <NoteStore.h>

using namespace boost;
using namespace apache;
using namespace evernote;

ResourceOperation::ResourceOperation(const edam::Resource& resource, Mode mode) :
    NetworkOperation(mode), m_resource(resource)
{
}

ResourceOperation::~ResourceOperation()
{
}

void ResourceOperation::operate(shared_ptr<thrift::protocol::TProtocol> protocol)
{
    edam::NoteStoreClient client(protocol);
    std::string token = authToken().toStdString();
    switch (mode())
    {
        case FetchResource:
            client.getResource(m_resource, token, m_resource.guid, true, false, true, false);
            emit fetched(m_resource);
            break;
        default:
            Q_ASSERT(false);
            break;
    }
}
