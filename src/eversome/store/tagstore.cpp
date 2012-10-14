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

#include "tagstore.h"
#include "tagoperation.h"
#include <QMetaType>
#include <QtDebug>

using namespace evernote;

Q_DECLARE_METATYPE(QVector<evernote::edam::Tag>)
Q_DECLARE_METATYPE(evernote::edam::Tag)

TagStore::TagStore(Session* session) : AbstractStore(session)
{
    qRegisterMetaType<QVector<evernote::edam::Tag> >();
    qRegisterMetaType<evernote::edam::Tag>();
}

TagStore::~TagStore()
{
}

void TagStore::create(const edam::Tag& tag)
{
    TagOperation* operation = new TagOperation(tag, Operation::CreateTag);
    connect(operation, SIGNAL(created(evernote::edam::Tag)),
                 this, SIGNAL(created(evernote::edam::Tag)));
    startOperation(operation);
}

void TagStore::fetch(const edam::Tag& tag)
{
    TagOperation* operation = new TagOperation(tag, Operation::FetchTag);
    connect(operation, SIGNAL(fetched(evernote::edam::Tag)),
                 this, SIGNAL(fetched(evernote::edam::Tag)));
    startOperation(operation);
}

void TagStore::rename(const evernote::edam::Tag& tag)
{
    evernote::edam::Tag modified;
    modified.guid = tag.guid;
    modified.name = tag.name;
    modified.__isset.guid = true; // :(
    modified.__isset.name = true; // :(

    TagOperation* operation = new TagOperation(modified, Operation::RenameTag);
    connect(operation, SIGNAL(renamed(evernote::edam::Tag)),
                 this, SIGNAL(renamed(evernote::edam::Tag)));
    startOperation(operation);
}
