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
    startNoteOperation(note, Operation::CreateNote);
}

void NoteStore::deleteNote(const edam::Note& note)
{
    startNoteOperation(note, Operation::DeleteNote);
}

void NoteStore::getNote(const edam::Note& note)
{
    startNoteOperation(note, Operation::GetNote);
}

void NoteStore::shareNote(const edam::Note& note)
{
    startNoteOperation(note, Operation::ShareNote);
}

void NoteStore::unshareNote(const edam::Note& note)
{
    startNoteOperation(note, Operation::UnshareNote);
}

void NoteStore::updateNote(const edam::Note& note)
{
    startNoteOperation(note, Operation::UpdateNote);
}

void NoteStore::createNotebook(const edam::Notebook& notebook)
{
    startNotebookOperation(notebook, Operation::CreateNotebook);
}

void NoteStore::getDefaultNotebook(const edam::Notebook& notebook)
{
    startNotebookOperation(notebook, Operation::GetDefaultNotebook);
}

void NoteStore::updateNotebook(const edam::Notebook& notebook)
{
    startNotebookOperation(notebook, Operation::UpdateNotebook);
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

void NoteStore::startNoteOperation(const edam::Note& note, Operation::Mode mode)
{
    NoteOperation* operation = new NoteOperation(note, mode);
    connect(operation, SIGNAL(noteFetched(evernote::edam::Note)),
                 this, SIGNAL(noteFetched(evernote::edam::Note)));
    connect(operation, SIGNAL(resourceFetched(evernote::edam::Resource)),
                 this, SIGNAL(resourceFetched(evernote::edam::Resource)));
    setupOperation(operation);
    qDebug() << "NoteStore::startNoteOperation():" << operation;
    QThreadPool::globalInstance()->start(operation);
}

void NoteStore::startNotebookOperation(const edam::Notebook& notebook, Operation::Mode mode)
{
    NotebookOperation* operation = new NotebookOperation(notebook, mode);
    setupOperation(operation);
    qDebug() << "NoteStore::startNotebookOperation():" << operation;
    QThreadPool::globalInstance()->start(operation);
}

void NoteStore::setupOperation(NetworkOperation* operation) const
{
    connect(operation, SIGNAL(started()), this, SIGNAL(activityChanged()));
    connect(operation, SIGNAL(finished()), this, SIGNAL(activityChanged()));
    connect(operation, SIGNAL(error(QString)), this, SIGNAL(error(QString)));
    operation->setUrl(session->url());
    operation->setAuthToken(session->authToken());
}
