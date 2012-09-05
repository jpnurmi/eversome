//#define QT_NO_DEBUG_OUTPUT

#include "notestore.h"
#include "userstore.h"
#include "settings.h"
#include "noteitem.h"
#include "manager.h"
#include <QtConcurrentRun>
#include <QMetaType>
#include <QtDebug>
#include "thrift/transport/THttpClient.h"
#include "thrift/protocol/TBinaryProtocol.h"
#include "edam/Limits_constants.h"

using namespace apache::thrift;
using namespace apache::thrift::protocol;
using namespace apache::thrift::transport;

Q_DECLARE_METATYPE(QVector<evernote::edam::SavedSearch>)
Q_DECLARE_METATYPE(QVector<evernote::edam::Notebook>)
Q_DECLARE_METATYPE(QVector<evernote::edam::Resource>)
Q_DECLARE_METATYPE(QVector<evernote::edam::Note>)
Q_DECLARE_METATYPE(QVector<evernote::edam::Tag>)
Q_DECLARE_METATYPE(evernote::edam::SavedSearch)
Q_DECLARE_METATYPE(evernote::edam::Resource)
Q_DECLARE_METATYPE(evernote::edam::Note)

NoteStore::NoteStore(UserStore *userStore) : QObject(userStore),
    syncing(false), fetching(false), searching(false), cancelled(false), userStore(userStore), client(0)
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
    delete client;
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

void NoteStore::fetch(const evernote::edam::Note& note)
{
    qDebug() << Q_FUNC_INFO;
    QtConcurrent::run(this, &NoteStore::fetchImpl, note);
}

void NoteStore::search(const evernote::edam::SavedSearch& search)
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
        init(true);
        int percent = 0;
        int usn = Settings::value(Settings::ServerUSN).toInt();

        evernote::edam::SyncChunk chunk;
        while (!cancelled) {
            emit progress(percent);
            std::string token = userStore->authToken().toStdString();
            client->getSyncChunk(chunk, token, usn, 1024, false);

            if (usn >= chunk.updateCount)
                break;

            percent = (int)((double)(100*(double)usn/(double)chunk.updateCount));
            emit progress(percent);

            QVector<evernote::edam::Notebook> notebooks = QVector<evernote::edam::Notebook>::fromStdVector(chunk.notebooks);
            QVector<evernote::edam::Resource> resources = QVector<evernote::edam::Resource>::fromStdVector(chunk.resources);
            QVector<evernote::edam::SavedSearch> searches = QVector<evernote::edam::SavedSearch>::fromStdVector(chunk.searches);
            QVector<evernote::edam::Note> notes = QVector<evernote::edam::Note>::fromStdVector(chunk.notes);
            QVector<evernote::edam::Tag> tags = QVector<evernote::edam::Tag>::fromStdVector(chunk.tags);

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
    } catch (evernote::edam::EDAMUserException& e) {
        err = Manager::errorString(e.errorCode);
    } catch (evernote::edam::EDAMSystemException& e) {
        err = Manager::errorString(e.errorCode);
    } catch (TException& e) {
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

void NoteStore::fetchImpl(const evernote::edam::Note& note)
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
        init(false);
        evernote::edam::Note copy(note);
        std::string token = userStore->authToken().toStdString();
        client->getNoteContent(copy.content, token, copy.guid);

        if (!copy.content.empty())
            emit noteFetched(copy);

        for (uint i = 0; !cancelled && i < copy.resources.size(); ++i) {
            evernote::edam::Resource res = copy.resources.at(i);
            client->getResource(res, token, res.guid, true, false, false, false);
            emit resourceFetched(res);
        }
    } catch (evernote::edam::EDAMUserException& e) {
        err = Manager::errorString(e.errorCode);
    } catch (evernote::edam::EDAMSystemException& e) {
        err = Manager::errorString(e.errorCode);
    } catch (evernote::edam::EDAMNotFoundException& e) {
        err = Manager::errorString(-1);
    } catch (TException& e) {
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

void NoteStore::searchImpl(const evernote::edam::SavedSearch& search)
{
    if (searching)
        return;

    qDebug() << Q_FUNC_INFO << QString::fromStdString(search.name);

    searching = true;
    emit isActiveChanged();

    QString err;
    try {
        init(false);
        evernote::edam::NoteList list;
        evernote::edam::NoteFilter filter;
        filter.words = search.query;
        std::string token = userStore->authToken().toStdString();
        client->findNotes(list, token, filter, 0, evernote::limits::g_Limits_constants.EDAM_USER_NOTES_MAX);

        if (list.notes.size())
            emit searched(search, QVector<evernote::edam::Note>::fromStdVector(list.notes));

    } catch (evernote::edam::EDAMUserException& e) {
        err = Manager::errorString(e.errorCode);
    } catch (evernote::edam::EDAMSystemException& e) {
        err = Manager::errorString(e.errorCode);
    } catch (evernote::edam::EDAMNotFoundException& e) {
        err = Manager::errorString(-1);
    } catch (TException& e) {
        err = QString::fromUtf8(e.what());
    }

    if (!err.isEmpty()) {
        qDebug() << Q_FUNC_INFO << err;
        emit error(err);
    }

    searching = false;
    emit isActiveChanged();
}

void NoteStore::init(bool force)
{
    if (force) {
        if (transport && transport->isOpen())
            transport->close();

        delete client;
        client = 0;
    }

    if (!client) {
        std::string host = Settings::value(Settings::Hostname).toStdString();
        int port = Settings::value(Settings::ServerPort).toInt();
        std::string url = userStore->notesUrl().toStdString();
        transport = boost::shared_ptr<TTransport>(new THttpClient(host, port, url));
        client = new evernote::edam::NoteStoreClient(boost::shared_ptr<TProtocol>(new TBinaryProtocol(transport)));
    }

    if (!transport->isOpen())
        transport->open();
}
