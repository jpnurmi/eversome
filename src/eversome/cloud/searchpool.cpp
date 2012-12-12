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

#include "searchpool.h"
#include "searchoperation.h"
#include <QMetaType>
#include <QtDebug>

using namespace evernote;

Q_DECLARE_METATYPE(QVector<evernote::edam::NoteMetadata>)
Q_DECLARE_METATYPE(QVector<evernote::edam::SavedSearch>)
Q_DECLARE_METATYPE(evernote::edam::NoteMetadata)
Q_DECLARE_METATYPE(evernote::edam::SavedSearch)

SearchPool::SearchPool(Session* session) : NetworkPool(session)
{
    qRegisterMetaType<QVector<evernote::edam::NoteMetadata> >();
    qRegisterMetaType<QVector<evernote::edam::SavedSearch> >();
    qRegisterMetaType<evernote::edam::NoteMetadata>();
    qRegisterMetaType<evernote::edam::SavedSearch>();
}

SearchPool::~SearchPool()
{
}

void SearchPool::create(const edam::SavedSearch& search)
{
    SearchOperation* operation = new SearchOperation(search, NetworkOperation::CreateSearch);
    connect(operation, SIGNAL(created(evernote::edam::SavedSearch)),
                 this, SIGNAL(created(evernote::edam::SavedSearch)));
    startOperation(operation, "notestore");
}

void SearchPool::fetch(const edam::SavedSearch& search)
{
    SearchOperation* operation = new SearchOperation(search, NetworkOperation::FetchSearch);
    connect(operation, SIGNAL(fetched(evernote::edam::SavedSearch)),
                 this, SIGNAL(fetched(evernote::edam::SavedSearch)));
    startOperation(operation, "notestore");
}

void SearchPool::search(const edam::SavedSearch& search)
{
    SearchOperation* operation = new SearchOperation(search, NetworkOperation::PerformSearch);
    connect(operation, SIGNAL(searched(evernote::edam::SavedSearch,QVector<evernote::edam::SavedSearchMetadata>)),
                 this, SIGNAL(searched(evernote::edam::SavedSearch,QVector<evernote::edam::SavedSearchMetadata>)));
    startOperation(operation, "notestore");
}

void SearchPool::rename(const evernote::edam::SavedSearch& search)
{
    evernote::edam::SavedSearch modified;
    modified.guid = search.guid;
    modified.name = search.name;
    modified.__isset.guid = true; // :(
    modified.__isset.name = true; // :(

    SearchOperation* operation = new SearchOperation(modified, NetworkOperation::RenameSearch);
    connect(operation, SIGNAL(renamed(evernote::edam::SavedSearch)),
                 this, SIGNAL(renamed(evernote::edam::SavedSearch)));
    startOperation(operation, "notestore");
}
