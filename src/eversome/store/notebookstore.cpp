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

#include "notebookstore.h"
#include "notebookoperation.h"
#include <QMetaType>
#include <QtDebug>

using namespace evernote;

Q_DECLARE_METATYPE(QVector<evernote::edam::Notebook>)
Q_DECLARE_METATYPE(evernote::edam::Notebook)

NotebookStore::NotebookStore(Session* session) : AbstractStore(session)
{
    qRegisterMetaType<QVector<evernote::edam::Notebook> >();
    qRegisterMetaType<evernote::edam::Notebook>();
}

NotebookStore::~NotebookStore()
{
}

void NotebookStore::create(const edam::Notebook& notebook)
{
    NotebookOperation* operation = new NotebookOperation(notebook, Operation::CreateNotebook);
    connect(operation, SIGNAL(created(evernote::edam::Notebook)),
                 this, SIGNAL(created(evernote::edam::Notebook)));
    qDebug() << "NotebookStore::create():" << operation;
    startOperation(operation);
}

void NotebookStore::fetch(const evernote::edam::Notebook& notebook)
{
    NotebookOperation* operation = new NotebookOperation(notebook, Operation::FetchNotebook);
    connect(operation, SIGNAL(fetched(evernote::edam::Notebook)),
                 this, SIGNAL(fetched(evernote::edam::Notebook)));
    qDebug() << "NotebookStore::fetch():" << operation;
    startOperation(operation);
}

void NotebookStore::rename(const evernote::edam::Notebook& notebook)
{
    evernote::edam::Notebook modified;
    modified.guid = notebook.guid;
    modified.name = notebook.name;
    modified.__isset.guid = true; // :(
    modified.__isset.name = true; // :(

    NotebookOperation* operation = new NotebookOperation(modified, Operation::RenameNotebook);
    connect(operation, SIGNAL(renamed(evernote::edam::Notebook)),
                 this, SIGNAL(renamed(evernote::edam::Notebook)));
    qDebug() << "NotebookStore::rename():" << operation;
    startOperation(operation);
}
