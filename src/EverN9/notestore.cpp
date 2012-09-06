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
#include "userstore.h"
#include "settings.h"
#include "noteitem.h"
#include "manager.h"
#include "noteoperation.h"
#include "syncoperation.h"
#include "searchoperation.h"
#include <QThreadPool>
#include <QMetaType>
#include <QtDebug>

using namespace boost;
using namespace apache;
using namespace evernote;

Q_DECLARE_METATYPE(QVector<evernote::edam::SavedSearch>)
Q_DECLARE_METATYPE(QVector<evernote::edam::Notebook>)
Q_DECLARE_METATYPE(QVector<evernote::edam::Resource>)
Q_DECLARE_METATYPE(QVector<evernote::edam::Note>)
Q_DECLARE_METATYPE(QVector<evernote::edam::Tag>)
Q_DECLARE_METATYPE(evernote::edam::SavedSearch)
Q_DECLARE_METATYPE(evernote::edam::Resource)
Q_DECLARE_METATYPE(evernote::edam::Note)

NoteStore::NoteStore(UserStore *userStore) : QObject(userStore),
    syncing(false), fetching(false), searching(false), cancelled(false), userStore(userStore)
{
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

bool NoteStore::isActive() const
{
    return syncing || fetching;
}

void NoteStore::sync()
{
    int usn = Settings::value(Settings::ServerUSN).toInt();
    SyncOperation* operation = new SyncOperation(usn);
    setupOperation(operation);
    qDebug() << Q_FUNC_INFO << operation;
    QThreadPool::globalInstance()->start(operation);
}

void NoteStore::cancel()
{
    qDebug() << Q_FUNC_INFO;
    cancelled = true;
}

void NoteStore::fetch(const edam::Note& note)
{
    NoteOperation* operation = new NoteOperation(note, NoteOperation::GetNote);
    setupOperation(operation);
    qDebug() << Q_FUNC_INFO << operation;
    QThreadPool::globalInstance()->start(operation);
}

void NoteStore::search(const edam::SavedSearch& search)
{
    SearchOperation* operation = new SearchOperation(search);
    setupOperation(operation);
    qDebug() << Q_FUNC_INFO << operation;
    QThreadPool::globalInstance()->start(operation);
}

void NoteStore::onOperationStarted(BaseOperation* operation)
{
    qDebug() << Q_FUNC_INFO << operation;
}

void NoteStore::onOperationFinished(BaseOperation* operation)
{
    qDebug() << Q_FUNC_INFO << operation;

    NoteOperation* noteOperation = qobject_cast<NoteOperation*>(operation);
    if (noteOperation && noteOperation->operation() == NoteOperation::GetNote) {
        edam::Note note = noteOperation->note();
        emit noteFetched(note);
        for (uint i = 0; i < note.resources.size(); ++i)
            emit resourceFetched(note.resources.at(i));
    }

    SyncOperation* syncOperation = qobject_cast<SyncOperation*>(operation);
    if (syncOperation) {
        Settings::setValue(Settings::ServerUSN, QString::number(syncOperation->usn()));
        emit synced(syncOperation->notebooks(),
                    syncOperation->resources(),
                    syncOperation->searches(),
                    syncOperation->notes(),
                    syncOperation->tags());
    }

    SearchOperation* searchOperation = qobject_cast<SearchOperation*>(operation);
    if (searchOperation)
        emit searched(searchOperation->search(), searchOperation->notes());

    operation->deleteLater();
}

void NoteStore::onOperationError(BaseOperation* operation, const QString& error)
{
    qDebug() << Q_FUNC_INFO << operation << error;
}

void NoteStore::setupOperation(BaseOperation* operation) const
{
    connect(operation, SIGNAL(started(BaseOperation*)),
                 this, SLOT(onOperationStarted(BaseOperation*)), Qt::DirectConnection);
    connect(operation, SIGNAL(finished(BaseOperation*)),
                 this, SLOT(onOperationFinished(BaseOperation*)), Qt::DirectConnection);
    connect(operation, SIGNAL(error(BaseOperation*,QString)),
                 this, SLOT(onOperationError(BaseOperation*,QString)), Qt::DirectConnection);
    operation->setHost(Settings::value(Settings::Hostname));
    operation->setPort(Settings::value(Settings::ServerPort).toInt());
    operation->setPath(userStore->notesUrl().path());
    operation->setAuthToken(userStore->authToken());
}
