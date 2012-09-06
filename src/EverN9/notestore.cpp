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
#include <QtConcurrentRun>
#include <QMetaType>
#include <QtDebug>
#include <Limits_constants.h>
#include <thrift/transport/THttpClient.h>
#include <thrift/protocol/TBinaryProtocol.h>

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
    qDebug() << Q_FUNC_INFO;
    QtConcurrent::run(this, &NoteStore::syncImpl);
}

void NoteStore::cancel()
{
    qDebug() << Q_FUNC_INFO;
    cancelled = true;
}

void NoteStore::fetch(const edam::Note& note)
{
    NoteOperation* operation = createOperation(note, NoteOperation::GetNote);
    qDebug() << Q_FUNC_INFO << operation;
    QThreadPool::globalInstance()->start(operation);
}

void NoteStore::search(const edam::SavedSearch& search)
{
    qDebug() << Q_FUNC_INFO;
    QtConcurrent::run(this, &NoteStore::searchImpl, search);
}

void NoteStore::syncImpl()
{
    if (syncing)
        return;

    qDebug() << Q_FUNC_INFO;

    syncing = true;
    cancelled = false;
    emit started();
    emit isActiveChanged();

    QString err;
    try {
        edam::NoteStoreClient client(createProtocol());

        int percent = 0;
        int usn = Settings::value(Settings::ServerUSN).toInt();
        std::string token = userStore->authToken().toStdString();

        while (!cancelled) {
            emit progress(percent);

            edam::SyncChunk chunk;
            client.getSyncChunk(chunk, token, usn, 256, false);

            if (usn >= chunk.updateCount)
                break;

            percent = (int)((double)(100*(double)usn/(double)chunk.updateCount));
            emit progress(percent);

            QVector<edam::Notebook> notebooks = QVector<edam::Notebook>::fromStdVector(chunk.notebooks);
            QVector<edam::Resource> resources = QVector<edam::Resource>::fromStdVector(chunk.resources);
            QVector<edam::SavedSearch> searches = QVector<edam::SavedSearch>::fromStdVector(chunk.searches);
            QVector<edam::Note> notes = QVector<edam::Note>::fromStdVector(chunk.notes);
            QVector<edam::Tag> tags = QVector<edam::Tag>::fromStdVector(chunk.tags);

            qDebug() << Q_FUNC_INFO
                     << "NB:" << notebooks.size()
                     << "R:" << resources.size()
                     << "S:" << searches.size()
                     << "N:" << notes.size()
                     << "T:" << tags.size()
                     << "USN:" << chunk.chunkHighUSN;

            emit synced(notebooks, resources, searches, notes, tags);

            usn = chunk.chunkHighUSN;
            Settings::setValue(Settings::ServerUSN, QString::number(usn));
            if (usn >= chunk.updateCount)
                break;
        }
    } catch (edam::EDAMUserException& e) {
        err = Manager::errorString(e.errorCode);
    } catch (edam::EDAMSystemException& e) {
        err = Manager::errorString(e.errorCode);
    } catch (thrift::TException& e) {
        err = QString::fromUtf8(e.what());
    }

    if (!err.isEmpty()) {
        qDebug() << Q_FUNC_INFO << err;
        emit error(err);
    }

    syncing = false;
    emit isActiveChanged();
    if (!cancelled)
        emit finished();
}

void NoteStore::searchImpl(const edam::SavedSearch& search)
{
    if (searching)
        return;

    qDebug() << Q_FUNC_INFO << QString::fromStdString(search.name);

    searching = true;
    emit isActiveChanged();

    QString err;
    try {
        edam::NoteList list;
        edam::NoteFilter filter;
        filter.words = search.query;
        std::string token = userStore->authToken().toStdString();

        edam::NoteStoreClient client(createProtocol());
        client.findNotes(list, token, filter, 0, limits::g_Limits_constants.EDAM_USER_NOTES_MAX);

        if (list.notes.size())
            emit searched(search, QVector<edam::Note>::fromStdVector(list.notes));

    } catch (edam::EDAMUserException& e) {
        err = Manager::errorString(e.errorCode);
    } catch (edam::EDAMSystemException& e) {
        err = Manager::errorString(e.errorCode);
    } catch (edam::EDAMNotFoundException& e) {
        err = Manager::errorString(-1);
    } catch (thrift::TException& e) {
        err = QString::fromUtf8(e.what());
    }

    if (!err.isEmpty()) {
        qDebug() << Q_FUNC_INFO << err;
        emit error(err);
    }

    searching = false;
    emit isActiveChanged();
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
    operation->deleteLater();
}

void NoteStore::onOperationError(BaseOperation* operation, const QString& error)
{
    qDebug() << Q_FUNC_INFO << operation << error;
}

NoteOperation* NoteStore::createOperation(const evernote::edam::Note& note, const  NoteOperation::Operation type) const
{
    NoteOperation* operation = new NoteOperation(note, type);
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
    return operation;
}

shared_ptr<apache::thrift::protocol::TProtocol> NoteStore::createProtocol() const
{
    std::string host = Settings::value(Settings::Hostname).toStdString();
    int port = Settings::value(Settings::ServerPort).toInt();
    std::string path = userStore->notesUrl().path().toStdString();

    shared_ptr<thrift::transport::TTransport> transport(new thrift::transport::THttpClient(host, port, path));
    transport->open();

    return shared_ptr<thrift::protocol::TProtocol>(new thrift::protocol::TBinaryProtocol(transport));
}
