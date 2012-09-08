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
Q_DECLARE_METATYPE(evernote::edam::Resource)
Q_DECLARE_METATYPE(evernote::edam::Note)

NoteStore::NoteStore(Session* session) : QObject(session), session(session)
{
    connect(session, SIGNAL(established()), SLOT(sync()));

    qRegisterMetaType<QVector<evernote::edam::SavedSearch> >();
    qRegisterMetaType<QVector<evernote::edam::Notebook> >();
    qRegisterMetaType<QVector<evernote::edam::Resource> >();
    qRegisterMetaType<QVector<evernote::edam::Note> >();
    qRegisterMetaType<QVector<evernote::edam::Tag> >();
    qRegisterMetaType<evernote::edam::SavedSearch>();
    qRegisterMetaType<evernote::edam::Resource>();
    qRegisterMetaType<evernote::edam::Note>();
}

NoteStore::~NoteStore()
{
}

QDateTime NoteStore::currentTime() const
{
    return dateTime;
}

void NoteStore::sync()
{
    int usn = QSettings().value("usn", 0).toInt();
    SyncOperation* operation = new SyncOperation(usn);
    setupOperation(operation);
    qDebug() << Q_FUNC_INFO << operation;
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
    setupOperation(operation);
    qDebug() << Q_FUNC_INFO << operation;
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

void NoteStore::expungeNote(const edam::Note& note)
{
    startNoteOperation(note, Operation::ExpungeNote);
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

void NoteStore::expungeNotebook(const edam::Notebook& notebook)
{
    startNotebookOperation(notebook, Operation::ExpungeNotebook);
}

void NoteStore::getDefaultNotebook(const edam::Notebook& notebook)
{
    startNotebookOperation(notebook, Operation::GetDefaultNotebook);
}

void NoteStore::updateNotebook(const edam::Notebook& notebook)
{
    startNotebookOperation(notebook, Operation::UpdateNotebook);
}

void NoteStore::onOperationStarted(Operation* operation)
{
    qDebug() << Q_FUNC_INFO << operation;
    emit activityChanged();
}

void NoteStore::onOperationFinished(Operation* operation)
{
    qDebug() << Q_FUNC_INFO << operation;

    NoteOperation* noteOperation = qobject_cast<NoteOperation*>(operation);
    if (noteOperation && noteOperation->mode() == NoteOperation::GetNote) {
        edam::Note note = noteOperation->note();
        emit noteFetched(note);
        for (uint i = 0; i < note.resources.size(); ++i)
            emit resourceFetched(note.resources.at(i));
    }

    SyncOperation* syncOperation = qobject_cast<SyncOperation*>(operation);
    if (syncOperation) {
        QSettings().setValue("usn", syncOperation->usn());
        const evernote::edam::SyncChunk& chunk = syncOperation->chunk();

        dateTime = QDateTime::fromMSecsSinceEpoch(chunk.currentTime);
        emit currentTimeChanged();

        emit synced(QVector<edam::Notebook>::fromStdVector(chunk.notebooks),
                    QVector<edam::Resource>::fromStdVector(chunk.resources),
                    QVector<edam::SavedSearch>::fromStdVector(chunk.searches),
                    QVector<edam::Note>::fromStdVector(chunk.notes),
                    QVector<edam::Tag>::fromStdVector(chunk.tags));
    }

    SearchOperation* searchOperation = qobject_cast<SearchOperation*>(operation);
    if (searchOperation)
        emit searched(searchOperation->search(), searchOperation->notes());

    operation->deleteLater();

    emit activityChanged();
}

void NoteStore::onOperationError(Operation* operation, const QString& str)
{
    qDebug() << Q_FUNC_INFO << operation << str;
    emit error(str);
}

void NoteStore::startNoteOperation(const edam::Note& note, Operation::Mode mode)
{
    NoteOperation* operation = new NoteOperation(note, mode);
    setupOperation(operation);
    qDebug() << Q_FUNC_INFO << operation;
    QThreadPool::globalInstance()->start(operation);
}

void NoteStore::startNotebookOperation(const edam::Notebook& notebook, Operation::Mode mode)
{
    NotebookOperation* operation = new NotebookOperation(notebook, mode);
    setupOperation(operation);
    qDebug() << Q_FUNC_INFO << operation;
    QThreadPool::globalInstance()->start(operation);
}

void NoteStore::setupOperation(NetworkOperation* operation) const
{
    connect(operation, SIGNAL(started(Operation*)),
                 this, SLOT(onOperationStarted(Operation*)), Qt::DirectConnection);
    connect(operation, SIGNAL(finished(Operation*)),
                 this, SLOT(onOperationFinished(Operation*)), Qt::DirectConnection);
    connect(operation, SIGNAL(error(Operation*,QString)),
                 this, SLOT(onOperationError(Operation*,QString)), Qt::DirectConnection);
    operation->setUrl(session->url());
    operation->setAuthToken(session->authToken());
}
