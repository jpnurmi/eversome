//#define QT_NO_DEBUG_OUTPUT

#include "notestore.h"
#include "settings.h"
#include "noteitem.h"
#include "manager.h"
#include <QtConcurrentRun>
#include <QMetaType>
#include <QtDebug>
#include "thrift/transport/THttpClient.h"
#include "thrift/protocol/TBinaryProtocol.h"

using namespace apache::thrift;
using namespace apache::thrift::protocol;
using namespace apache::thrift::transport;

static const std::string EDAM_HOST = "www.evernote.com";
static const std::string EDAM_ROOT = "/edam/note/";
static const int EDAM_PORT = 80;

Q_DECLARE_METATYPE(QVector<evernote::edam::Notebook>)
Q_DECLARE_METATYPE(QVector<evernote::edam::Resource>)
Q_DECLARE_METATYPE(QVector<evernote::edam::Note>)
Q_DECLARE_METATYPE(QVector<evernote::edam::Tag>)
Q_DECLARE_METATYPE(evernote::edam::Resource)
Q_DECLARE_METATYPE(evernote::edam::Note)

NoteStore::NoteStore(QObject *parent) : QObject(parent),
    syncing(false), fetching(false), cancelled(false), client(0)
{
    qRegisterMetaType<QVector<evernote::edam::Notebook> >();
    qRegisterMetaType<QVector<evernote::edam::Resource> >();
    qRegisterMetaType<QVector<evernote::edam::Note> >();
    qRegisterMetaType<QVector<evernote::edam::Tag> >();
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

void NoteStore::syncImpl()
{
    if (syncing)
        return;

    qDebug() << Q_FUNC_INFO;

    syncing = true;
    cancelled = false;
    emit started();
    emit activeChanged();

    QString err;
    try {
        init(true);
        int i = 0;
        while (!cancelled && ++i <= 5) {
            int percent = 0;
            int usn = Settings::value(Settings::ServerUSN).toInt();

            init(false);

            evernote::edam::SyncChunk chunk;
            while (!cancelled) {
                emit progress(percent);
                client->getSyncChunk(chunk, Settings::value(Settings::AuthToken).toStdString(), usn, 1024, false);

                if (usn >= chunk.updateCount)
                    break;

                percent = (int)((double)(100*(double)usn/(double)chunk.updateCount));
                emit progress(percent);

                QVector<evernote::edam::Notebook> notebooks = QVector<evernote::edam::Notebook>::fromStdVector(chunk.notebooks);
                QVector<evernote::edam::Resource> resources = QVector<evernote::edam::Resource>::fromStdVector(chunk.resources);
                QVector<evernote::edam::Note> notes = QVector<evernote::edam::Note>::fromStdVector(chunk.notes);
                QVector<evernote::edam::Tag> tags = QVector<evernote::edam::Tag>::fromStdVector(chunk.tags);

                qDebug() << Q_FUNC_INFO
                         << "NB:" << notebooks.size()
                         << "R:" << resources.size()
                         << "N:" << notes.size()
                         << "T:" << tags.size()
                         << "USN:" << chunk.chunkHighUSN;

                emit synced(notebooks, resources, notes, tags);

                usn = chunk.chunkHighUSN;
                Settings::setValue(Settings::ServerUSN, QString::number(usn));
                if (usn >= chunk.updateCount)
                    break;
            }
            break;
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

    syncing = false;
    emit activeChanged();
    if (!cancelled)
        emit finished();
}

void NoteStore::fetchImpl(const evernote::edam::Note& note)
{
    if (fetching)
        return;

    qDebug() << Q_FUNC_INFO;

    fetching = true;
    cancelled = false;
    emit started();
    emit activeChanged();

    QString err;
    try {
        init(false);
        evernote::edam::Note copy(note);
        client->getNoteContent(copy.content, Settings::value(Settings::AuthToken).toStdString(), copy.guid);

        if (!copy.content.empty())
            emit noteFetched(copy);

        for (uint i = 0; !cancelled && i < copy.resources.size(); ++i) {
            evernote::edam::Resource res = copy.resources.at(i);
            client->getResource(res, Settings::value(Settings::AuthToken).toStdString(), res.guid, true, false, false, false);
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
    emit activeChanged();
    if (!cancelled)
        emit finished();
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
        QString shardId = Settings::value(Settings::UserShardID);
        transport = boost::shared_ptr<TTransport>(new THttpClient(EDAM_HOST, EDAM_PORT, EDAM_ROOT + shardId.toStdString()));
        client = new evernote::edam::NoteStoreClient(boost::shared_ptr<TProtocol>(new TBinaryProtocol(transport)));
    }

    if (!transport->isOpen())
        transport->open();
}
