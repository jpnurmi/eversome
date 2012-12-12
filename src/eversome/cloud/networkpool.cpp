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
#include "tagoperation.h"
#include "noteoperation.h"
#include "searchoperation.h"
#include "networkoperation.h"
#include "notebookoperation.h"
#include "resourceoperation.h"
#include "thumbnailoperation.h"
#include "session.h"

using namespace evernote;

Q_DECLARE_METATYPE(QVector<evernote::edam::Notebook>)
Q_DECLARE_METATYPE(evernote::edam::Notebook)

Q_DECLARE_METATYPE(QVector<evernote::edam::Note>)
Q_DECLARE_METATYPE(evernote::edam::Note)

Q_DECLARE_METATYPE(QVector<evernote::edam::Tag>)
Q_DECLARE_METATYPE(evernote::edam::Tag)

Q_DECLARE_METATYPE(QVector<evernote::edam::SavedSearch>)
Q_DECLARE_METATYPE(evernote::edam::SavedSearch)

Q_DECLARE_METATYPE(QVector<evernote::edam::NoteMetadata>)
Q_DECLARE_METATYPE(evernote::edam::NoteMetadata)

Q_DECLARE_METATYPE(QVector<evernote::edam::Resource>)
Q_DECLARE_METATYPE(evernote::edam::Resource)

NetworkPool::NetworkPool(Session* session) : AbstractPool(session), m_session(session)
{
    qRegisterMetaType<QVector<evernote::edam::Notebook> >();
    qRegisterMetaType<evernote::edam::Notebook>();

    qRegisterMetaType<QVector<evernote::edam::Note> >();
    qRegisterMetaType<evernote::edam::Note>();

    qRegisterMetaType<QVector<evernote::edam::Tag> >();
    qRegisterMetaType<evernote::edam::Tag>();

    qRegisterMetaType<QVector<evernote::edam::NoteMetadata> >();
    qRegisterMetaType<evernote::edam::NoteMetadata>();

    qRegisterMetaType<QVector<evernote::edam::SavedSearch> >();
    qRegisterMetaType<evernote::edam::SavedSearch>();

    qRegisterMetaType<QVector<evernote::edam::Resource> >();
    qRegisterMetaType<evernote::edam::Resource>();
}

NetworkPool::~NetworkPool()
{
}

Session* NetworkPool::session() const
{
    return m_session;
}

void NetworkPool::createNotebook(const edam::Notebook& notebook)
{
    NotebookOperation* operation = new NotebookOperation(notebook, NetworkOperation::CreateNotebook);
    connect(operation, SIGNAL(created(evernote::edam::Notebook)),
                 this, SIGNAL(created(evernote::edam::Notebook)));
    startOperation(operation, "notestore");
}

void NetworkPool::fetchNotebook(const evernote::edam::Notebook& notebook)
{
    NotebookOperation* operation = new NotebookOperation(notebook, NetworkOperation::FetchNotebook);
    connect(operation, SIGNAL(fetched(evernote::edam::Notebook)),
                 this, SIGNAL(fetched(evernote::edam::Notebook)));
    startOperation(operation, "notestore");
}

void NetworkPool::renameNotebook(const evernote::edam::Notebook& notebook)
{
    evernote::edam::Notebook modified;
    modified.guid = notebook.guid;
    modified.name = notebook.name;
    modified.__isset.guid = true; // :(
    modified.__isset.name = true; // :(

    NotebookOperation* operation = new NotebookOperation(modified, NetworkOperation::RenameNotebook);
    connect(operation, SIGNAL(renamed(evernote::edam::Notebook)),
                 this, SIGNAL(renamed(evernote::edam::Notebook)));
    startOperation(operation, "notestore");
}

void NetworkPool::createNote(const edam::Note& note)
{
    NoteOperation* operation = new NoteOperation(note, NetworkOperation::CreateNote);
    connect(operation, SIGNAL(created(evernote::edam::Note)),
                 this, SIGNAL(created(evernote::edam::Note)));
    startOperation(operation, "notestore");
}

void NetworkPool::fetchNote(const edam::Note& note)
{
    NoteOperation* operation = new NoteOperation(note, NetworkOperation::FetchNote);
    connect(operation, SIGNAL(fetched(evernote::edam::Note)),
                 this, SIGNAL(fetched(evernote::edam::Note)));
    startOperation(operation, "notestore");
}

void NetworkPool::moveNote(const evernote::edam::Note& note, const evernote::edam::Notebook& notebook)
{
    evernote::edam::Note modified;
    modified.guid = note.guid;
    modified.title = note.title;
    modified.notebookGuid = notebook.guid;
    modified.__isset.guid = true; // :(
    modified.__isset.title = true; // :(
    modified.__isset.notebookGuid = true; // :(

    NoteOperation* operation = new NoteOperation(modified, NetworkOperation::MoveNote);
    connect(operation, SIGNAL(moved(evernote::edam::Note)),
                 this, SIGNAL(moved(evernote::edam::Note)));
    startOperation(operation, "notestore");
}

void NetworkPool::renameNote(const evernote::edam::Note& note)
{
    evernote::edam::Note modified;
    modified.guid = note.guid;
    modified.title = note.title;
    modified.__isset.guid = true; // :(
    modified.__isset.title = true; // :(

    NoteOperation* operation = new NoteOperation(modified, NetworkOperation::RenameNote);
    connect(operation, SIGNAL(renamed(evernote::edam::Note)),
                 this, SIGNAL(renamed(evernote::edam::Note)));
    startOperation(operation, "notestore");
}

void NetworkPool::trashNote(const edam::Note& note)
{
    NoteOperation* operation = new NoteOperation(note, NetworkOperation::TrashNote);
    connect(operation, SIGNAL(trashed(evernote::edam::Note)),
                 this, SIGNAL(trashed(evernote::edam::Note)));
    startOperation(operation, "notestore");
}

void NetworkPool::createTag(const edam::Tag& tag)
{
    TagOperation* operation = new TagOperation(tag, NetworkOperation::CreateTag);
    connect(operation, SIGNAL(created(evernote::edam::Tag)),
                 this, SIGNAL(created(evernote::edam::Tag)));
    startOperation(operation, "notestore");
}

void NetworkPool::fetchTag(const edam::Tag& tag)
{
    TagOperation* operation = new TagOperation(tag, NetworkOperation::FetchTag);
    connect(operation, SIGNAL(fetched(evernote::edam::Tag)),
                 this, SIGNAL(fetched(evernote::edam::Tag)));
    startOperation(operation, "notestore");
}

void NetworkPool::renameTag(const evernote::edam::Tag& tag)
{
    evernote::edam::Tag modified;
    modified.guid = tag.guid;
    modified.name = tag.name;
    modified.__isset.guid = true; // :(
    modified.__isset.name = true; // :(

    TagOperation* operation = new TagOperation(modified, NetworkOperation::RenameTag);
    connect(operation, SIGNAL(renamed(evernote::edam::Tag)),
                 this, SIGNAL(renamed(evernote::edam::Tag)));
    startOperation(operation, "notestore");
}

void NetworkPool::createSearch(const edam::SavedSearch& search)
{
    SearchOperation* operation = new SearchOperation(search, NetworkOperation::CreateSearch);
    connect(operation, SIGNAL(created(evernote::edam::SavedSearch)),
                 this, SIGNAL(created(evernote::edam::SavedSearch)));
    startOperation(operation, "notestore");
}

void NetworkPool::fetchSearch(const edam::SavedSearch& search)
{
    SearchOperation* operation = new SearchOperation(search, NetworkOperation::FetchSearch);
    connect(operation, SIGNAL(fetched(evernote::edam::SavedSearch)),
                 this, SIGNAL(fetched(evernote::edam::SavedSearch)));
    startOperation(operation, "notestore");
}

void NetworkPool::search(const edam::SavedSearch& search)
{
    SearchOperation* operation = new SearchOperation(search, NetworkOperation::PerformSearch);
    connect(operation, SIGNAL(searched(evernote::edam::SavedSearch,QVector<evernote::edam::SavedSearchMetadata>)),
                 this, SIGNAL(searched(evernote::edam::SavedSearch,QVector<evernote::edam::SavedSearchMetadata>)));
    startOperation(operation, "notestore");
}

void NetworkPool::renameSearch(const evernote::edam::SavedSearch& search)
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

void NetworkPool::fetchResource(const edam::Resource& resource)
{
    ResourceOperation* operation = new ResourceOperation(resource, NetworkOperation::FetchResource);
    connect(operation, SIGNAL(fetched(evernote::edam::Resource)),
                 this, SIGNAL(fetched(evernote::edam::Resource)));
    startOperation(operation, "notestore");
}

void NetworkPool::fetchThumbnail(const edam::Note& note)
{
    QString guid = QString::fromStdString(note.guid);
    ThumbnailOperation* operation = new ThumbnailOperation(guid);
    connect(operation, SIGNAL(fetched(QString,QByteArray)),
                 this, SIGNAL(fetched(QString,QByteArray)));
    startOperation(operation, QString("thm/note/%1.png").arg(guid));
}

void NetworkPool::fetchThumbnail(const edam::Resource& resource)
{
    QString guid = QString::fromStdString(resource.guid);
    ThumbnailOperation* operation = new ThumbnailOperation(guid);
    connect(operation, SIGNAL(fetched(QString,QByteArray)),
                 this, SIGNAL(fetched(QString,QByteArray)));
    startOperation(operation, QString("thm/res/%1.png").arg(guid));
}

void NetworkPool::startOperation(NetworkOperation* operation, const QString& path)
{
    operation->setUrl(m_session->url(path));
    operation->setAuthToken(m_session->authToken());
    startOperation(operation);
}
