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

#include "abstractstore.h"
#include "networkoperation.h"
#include "session.h"
#include <QThreadPool>
#include <QtDebug>

AbstractStore::AbstractStore(Session* session) : QObject(session), m_session(session)
{
}

AbstractStore::~AbstractStore()
{
}

Session* AbstractStore::session() const
{
    return m_session;
}

void AbstractStore::startOperation(NetworkOperation* operation)
{
    connect(operation, SIGNAL(started()), this, SIGNAL(activityChanged()));
    connect(operation, SIGNAL(finished()), this, SIGNAL(activityChanged()));
    connect(operation, SIGNAL(error(QString)), this, SIGNAL(error(QString)));
    operation->setUrl(m_session->url());
    operation->setAuthToken(m_session->authToken());
    QThreadPool::globalInstance()->start(operation);
    qDebug() << metaObject()->className() << "::startOperation():" << operation;
}
