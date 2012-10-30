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

#include "notestore.h"
#include "noteoperation.h"
#include <QMetaType>
#include <QtDebug>

using namespace evernote;

Q_DECLARE_METATYPE(QVector<evernote::edam::Note>)
Q_DECLARE_METATYPE(evernote::edam::Note)

NoteStore::NoteStore(Session* session) : AbstractStore(session)
{
    qRegisterMetaType<QVector<evernote::edam::Note> >();
    qRegisterMetaType<evernote::edam::Note>();
}

NoteStore::~NoteStore()
{
}

void NoteStore::create(const edam::Note& note)
{
    NoteOperation* operation = new NoteOperation(note, Operation::CreateNote);
    connect(operation, SIGNAL(created(evernote::edam::Note)),
                 this, SIGNAL(created(evernote::edam::Note)));
    startOperation(operation);
}

void NoteStore::fetch(const edam::Note& note)
{
    NoteOperation* operation = new NoteOperation(note, Operation::FetchNote);
    connect(operation, SIGNAL(fetched(evernote::edam::Note)),
                 this, SIGNAL(fetched(evernote::edam::Note)));
    startOperation(operation);
}

void NoteStore::move(const evernote::edam::Note& note, const evernote::edam::Notebook& notebook)
{
    evernote::edam::Note modified;
    modified.guid = note.guid;
    modified.title = note.title;
    modified.notebookGuid = notebook.guid;
    modified.__isset.guid = true; // :(
    modified.__isset.title = true; // :(
    modified.__isset.notebookGuid = true; // :(

    NoteOperation* operation = new NoteOperation(modified, Operation::MoveNote);
    connect(operation, SIGNAL(moved(evernote::edam::Note)),
                 this, SIGNAL(moved(evernote::edam::Note)));
    startOperation(operation);
}

void NoteStore::rename(const evernote::edam::Note& note)
{
    evernote::edam::Note modified;
    modified.guid = note.guid;
    modified.title = note.title;
    modified.__isset.guid = true; // :(
    modified.__isset.title = true; // :(

    NoteOperation* operation = new NoteOperation(modified, Operation::RenameNote);
    connect(operation, SIGNAL(renamed(evernote::edam::Note)),
                 this, SIGNAL(renamed(evernote::edam::Note)));
    startOperation(operation);
}

void NoteStore::trash(const edam::Note& note)
{
    NoteOperation* operation = new NoteOperation(note, Operation::TrashNote);
    connect(operation, SIGNAL(trashed(evernote::edam::Note)),
                 this, SIGNAL(trashed(evernote::edam::Note)));
    startOperation(operation);
}
