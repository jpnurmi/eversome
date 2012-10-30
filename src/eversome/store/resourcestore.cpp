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

#include "resourcestore.h"
#include "resourceoperation.h"
#include <QMetaType>
#include <QtDebug>

using namespace evernote;

Q_DECLARE_METATYPE(QVector<evernote::edam::Resource>)
Q_DECLARE_METATYPE(evernote::edam::Resource)

ResourceStore::ResourceStore(Session* session) : AbstractStore(session)
{
    qRegisterMetaType<QVector<evernote::edam::Resource> >();
    qRegisterMetaType<evernote::edam::Resource>();
}

ResourceStore::~ResourceStore()
{
}

void ResourceStore::fetch(const edam::Resource& resource)
{
    ResourceOperation* operation = new ResourceOperation(resource, Operation::FetchResource);
    connect(operation, SIGNAL(fetched(evernote::edam::Resource)),
                 this, SIGNAL(fetched(evernote::edam::Resource)));
    startOperation(operation);
}
