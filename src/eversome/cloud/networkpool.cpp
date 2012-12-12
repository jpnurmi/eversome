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

#include "networkpool.h"
#include "networkoperation.h"
#include "session.h"

NetworkPool::NetworkPool(Session* session) : AbstractPool(session), m_session(session)
{
}

NetworkPool::~NetworkPool()
{
}

Session* NetworkPool::session() const
{
    return m_session;
}

void NetworkPool::startOperation(NetworkOperation* operation, const QString& path)
{
    operation->setUrl(m_session->url(path));
    operation->setAuthToken(m_session->authToken());
    startOperation(operation);
}
