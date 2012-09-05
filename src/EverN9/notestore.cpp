//#define QT_NO_DEBUG_OUTPUT

#include "notestore.h"
#include "userstore.h"
#include "settings.h"
#include "noteitem.h"
#include "manager.h"
#include <QtConcurrentRun>
#include <QMetaType>
#include <QtDebug>
#include "edam/Limits_constants.h"
#include "thrift/transport/THttpClient.h"
#include "thrift/protocol/TBinaryProtocol.h"

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
    qDebug() << Q_FUNC_INFO;
    QtConcurrent::run(this, &NoteStore::fetchImpl, note);
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

void NoteStore::fetchImpl(const edam::Note& note)
{
    if (fetching)
        return;

    qDebug() << Q_FUNC_INFO << QString::fromStdString(note.title);

    fetching = true;
    cancelled = false;
    emit started();
    emit isActiveChanged();

    QString err;
    try {
        edam::Note copy(note);
        std::string token = userStore->authToken().toStdString();

        edam::NoteStoreClient client(createProtocol());
        client.getNoteContent(copy.content, token, copy.guid);

        if (!copy.content.empty())
            emit noteFetched(copy);

        for (uint i = 0; !cancelled && i < copy.resources.size(); ++i) {
            edam::Resource res = copy.resources.at(i);
            client.getResource(res, token, res.guid, true, false, false, false);
            emit resourceFetched(res);
        }

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

    fetching = false;
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

shared_ptr<apache::thrift::protocol::TProtocol> NoteStore::createProtocol() const
{
    std::string host = Settings::value(Settings::Hostname).toStdString();
    int port = Settings::value(Settings::ServerPort).toInt();
    std::string path = userStore->notesUrl().path().toStdString();

    shared_ptr<thrift::transport::TTransport> transport(new thrift::transport::THttpClient(host, port, path));
    transport->open();

    return shared_ptr<thrift::protocol::TProtocol>(new thrift::protocol::TBinaryProtocol(transport));
}
