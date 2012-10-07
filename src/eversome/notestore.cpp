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
#include "noteitem.h"
#include "session.h"
#include "manager.h"
#include "noteoperation.h"
#include "syncoperation.h"
#include "searchoperation.h"
#include "notebookoperation.h"
#include <QThreadPool>
#include <QSettings>
#include <QMetaType>
#include <QtDebug>

using namespace evernote;

Q_DECLARE_METATYPE(QVector<evernote::edam::SavedSearch>)
Q_DECLARE_METATYPE(QVector<evernote::edam::Notebook>)
Q_DECLARE_METATYPE(QVector<evernote::edam::Resource>)
Q_DECLARE_METATYPE(QVector<evernote::edam::Note>)
Q_DECLARE_METATYPE(QVector<evernote::edam::Tag>)
Q_DECLARE_METATYPE(evernote::edam::SavedSearch)
Q_DECLARE_METATYPE(evernote::edam::Notebook)
Q_DECLARE_METATYPE(evernote::edam::Resource)
Q_DECLARE_METATYPE(evernote::edam::Note)
Q_DECLARE_METATYPE(evernote::edam::Tag)

NoteStore::NoteStore(Session* session) : QObject(session), session(session)
{
    connect(session, SIGNAL(established()), SLOT(sync()));

    qRegisterMetaType<QVector<evernote::edam::SavedSearch> >();
    qRegisterMetaType<QVector<evernote::edam::Notebook> >();
    qRegisterMetaType<QVector<evernote::edam::Resource> >();
    qRegisterMetaType<QVector<evernote::edam::Note> >();
    qRegisterMetaType<QVector<evernote::edam::Tag> >();
    qRegisterMetaType<evernote::edam::SavedSearch>();
    qRegisterMetaType<evernote::edam::Notebook>();
    qRegisterMetaType<evernote::edam::Resource>();
    qRegisterMetaType<evernote::edam::Note>();
    qRegisterMetaType<evernote::edam::Tag>();
}

NoteStore::~NoteStore()
{
}

int NoteStore::usn() const
{
    return QSettings().value("usn").toInt();
}

QDateTime NoteStore::currentTime() const
{
    return QSettings().value("time").toDateTime();
}

void NoteStore::sync()
{
    SyncOperation* operation = new SyncOperation(usn());
    connect(operation, SIGNAL(usnChanged(int)), this, SLOT(setUsn(int)));
    connect(operation, SIGNAL(currentTimeChanged(QDateTime)), this, SLOT(setCurrentTime(QDateTime)));
    connect(operation, SIGNAL(synced(QVector<evernote::edam::Notebook>,
                                     QVector<evernote::edam::Resource>,
                                     QVector<evernote::edam::SavedSearch>,
                                     QVector<evernote::edam::Note>,
                                     QVector<evernote::edam::Tag>)),
                 this, SIGNAL(synced(QVector<evernote::edam::Notebook>,
                                     QVector<evernote::edam::Resource>,
                                     QVector<evernote::edam::SavedSearch>,
                                     QVector<evernote::edam::Note>,
                                     QVector<evernote::edam::Tag>)));
    connect(operation, SIGNAL(expunged(QVector<std::string>,
                                       QVector<std::string>,
                                       QVector<std::string>,
                                       QVector<std::string>)),
                 this, SIGNAL(expunged(QVector<std::string>,
                                       QVector<std::string>,
                                       QVector<std::string>,
                                       QVector<std::string>)));
    setupOperation(operation);
    qDebug() << "NoteStore::sync():" << operation;
    QThreadPool::globalInstance()->start(operation);
}

void NoteStore::cancel()
{
    qDebug() << Q_FUNC_INFO;
    // TODO
}

void NoteStore::search(const edam::SavedSearch& search)
{
    SearchOperation* operation = new SearchOperation(search);
    connect(operation, SIGNAL(searched(evernote::edam::SavedSearch,QVector<evernote::edam::Note>)),
                 this, SIGNAL(searched(evernote::edam::SavedSearch,QVector<evernote::edam::Note>)));
    setupOperation(operation);
    qDebug() << "NoteStore::search():" << operation;
    QThreadPool::globalInstance()->start(operation);
}

void NoteStore::createNote(const edam::Note& note)
{
    NoteOperation* operation = new NoteOperation(note, Operation::CreateNote);
    connect(operation, SIGNAL(noteCreated(evernote::edam::Note)),
                 this, SIGNAL(noteCreated(evernote::edam::Note)));
    setupOperation(operation);
    qDebug() << "NoteStore::createNote():" << operation;
    QThreadPool::globalInstance()->start(operation);
}

void NoteStore::deleteNote(const edam::Note& note)
{
    NoteOperation* operation = new NoteOperation(note, Operation::DeleteNote);
    connect(operation, SIGNAL(noteDeleted(evernote::edam::Note)),
                 this, SIGNAL(noteDeleted(evernote::edam::Note)));
    setupOperation(operation);
    qDebug() << "NoteStore::deleteNote():" << operation;
    QThreadPool::globalInstance()->start(operation);
}

void NoteStore::getNote(const edam::Note& note)
{
    NoteOperation* operation = new NoteOperation(note, Operation::GetNote);
    connect(operation, SIGNAL(noteFetched(evernote::edam::Note)),
                 this, SIGNAL(noteFetched(evernote::edam::Note)));
    connect(operation, SIGNAL(resourceFetched(evernote::edam::Resource)),
                 this, SIGNAL(resourceFetched(evernote::edam::Resource)));
    setupOperation(operation);
    qDebug() << "NoteStore::getNote():" << operation;
    QThreadPool::globalInstance()->start(operation);
}

void NoteStore::moveNote(const evernote::edam::Note& note, const evernote::edam::Notebook& notebook)
{
    evernote::edam::Note tmp;
    tmp.guid = note.guid;
    tmp.title = note.title;
    tmp.notebookGuid = notebook.guid;
    updateNote(tmp);
}

void NoteStore::shareNote(const edam::Note& note)
{
    NoteOperation* operation = new NoteOperation(note, Operation::ShareNote);
    connect(operation, SIGNAL(noteShared(evernote::edam::Note)),
                 this, SIGNAL(noteShared(evernote::edam::Note)));
    setupOperation(operation);
    qDebug() << "NoteStore::shareNote():" << operation;
    QThreadPool::globalInstance()->start(operation);
}

void NoteStore::unshareNote(const edam::Note& note)
{
    NoteOperation* operation = new NoteOperation(note, Operation::UnshareNote);
    connect(operation, SIGNAL(noteUnshared(evernote::edam::Note)),
                 this, SIGNAL(noteUnshared(evernote::edam::Note)));
    setupOperation(operation);
    qDebug() << "NoteStore::unshareNote():" << operation;
    QThreadPool::globalInstance()->start(operation);
}

void NoteStore::updateNote(const edam::Note& note)
{
    NoteOperation* operation = new NoteOperation(note, Operation::UpdateNote);
    connect(operation, SIGNAL(noteUpdated(evernote::edam::Note)),
                 this, SIGNAL(noteUpdated(evernote::edam::Note)));
    setupOperation(operation);
    qDebug() << "NoteStore::updateNote():" << operation;
    QThreadPool::globalInstance()->start(operation);
}

void NoteStore::createNotebook(const edam::Notebook& notebook)
{
    NotebookOperation* operation = new NotebookOperation(notebook, Operation::CreateNotebook);
    connect(operation, SIGNAL(notebookCreated(evernote::edam::Notebook)),
                 this, SIGNAL(notebookCreated(evernote::edam::Notebook)));
    setupOperation(operation);
    qDebug() << "NoteStore::createNobook():" << operation;
    QThreadPool::globalInstance()->start(operation);
}

void NoteStore::getNotebook(const evernote::edam::Notebook& notebook)
{
    NotebookOperation* operation = new NotebookOperation(notebook, Operation::GetNotebook);
    connect(operation, SIGNAL(notebookFetched(evernote::edam::Notebook)),
                 this, SIGNAL(notebookFetched(evernote::edam::Notebook)));
    setupOperation(operation);
    qDebug() << "NoteStore::getNotebook():" << operation;
    QThreadPool::globalInstance()->start(operation);
}

void NoteStore::getDefaultNotebook(const edam::Notebook& notebook)
{
    NotebookOperation* operation = new NotebookOperation(notebook, Operation::GetDefaultNotebook);
    connect(operation, SIGNAL(defaultNotebookFetched(evernote::edam::Notebook)),
                 this, SIGNAL(defaultNotebookFetched(evernote::edam::Notebook)));
    setupOperation(operation);
    qDebug() << "NoteStore::getDefaultNotebook():" << operation;
    QThreadPool::globalInstance()->start(operation);
}

void NoteStore::updateNotebook(const edam::Notebook& notebook)
{
    NotebookOperation* operation = new NotebookOperation(notebook, Operation::UpdateNotebook);
    connect(operation, SIGNAL(notebookUpdated(evernote::edam::Notebook)),
                 this, SIGNAL(notebookUpdated(evernote::edam::Notebook)));
    setupOperation(operation);
    qDebug() << "NoteStore::updateNotebook():" << operation;
    QThreadPool::globalInstance()->start(operation);
}

void NoteStore::setUsn(int val)
{
    int old = usn();
    if (old != val) {
        QSettings().setValue("usn", val);
        emit usnChanged();
    }
}

void NoteStore::setCurrentTime(const QDateTime& val)
{
    QDateTime old = currentTime();
    if (old != val) {
        QSettings().setValue("time", val);
        emit currentTimeChanged();
    }
}

void NoteStore::setupOperation(NetworkOperation* operation) const
{
    connect(operation, SIGNAL(started()), this, SIGNAL(activityChanged()));
    connect(operation, SIGNAL(finished()), this, SIGNAL(activityChanged()));
    connect(operation, SIGNAL(error(QString)), this, SIGNAL(error(QString)));
    operation->setUrl(session->url());
    operation->setAuthToken(session->authToken());
}
