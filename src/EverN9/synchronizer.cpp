//#define QT_NO_DEBUG_OUTPUT

#include "synchronizer.h"
#include "settings.h"
#include "noteitem.h"
#include <QtConcurrentRun>
#include <QMetaType>
#include <QtDebug>
#include "thrift/transport/THttpClient.h"
#include "thrift/protocol/TBinaryProtocol.h"

using namespace evernote::edam;
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

Synchronizer::Synchronizer(QObject *parent) : QObject(parent),
    syncing(false), fetching(false), cancelled(false), client(0)
{
    qRegisterMetaType<QVector<evernote::edam::Notebook> >();
    qRegisterMetaType<QVector<evernote::edam::Resource> >();
    qRegisterMetaType<QVector<evernote::edam::Note> >();
    qRegisterMetaType<QVector<evernote::edam::Tag> >();
    qRegisterMetaType<evernote::edam::Resource>();
    qRegisterMetaType<evernote::edam::Note>();
}

Synchronizer::~Synchronizer()
{
    delete client;
}

bool Synchronizer::isActive() const
{
    return syncing || fetching;
}

void Synchronizer::sync()
{
    qDebug() << Q_FUNC_INFO;
    QtConcurrent::run(this, &Synchronizer::syncImpl);
}

void Synchronizer::cancel()
{
    qDebug() << Q_FUNC_INFO;
    cancelled = true;
}

void Synchronizer::fetch(const evernote::edam::Note& note)
{
    qDebug() << Q_FUNC_INFO;
    QtConcurrent::run(this, &Synchronizer::fetchImpl, note);
}

void Synchronizer::syncImpl()
{
    if (syncing)
        return;

    qDebug() << Q_FUNC_INFO;

    syncing = true;
    cancelled = false;
    emit started();
    emit activeChanged();

    try {
        init(true);
        int i = 0;
        while (!cancelled && ++i <= 5) {
            try {
                int percent = 0;
                int usn = Settings::value(Settings::ServerUSN).toInt();

                init(false);

                SyncChunk chunk;
                while (!cancelled) {
                    emit progress(percent);
                    client->getSyncChunk(chunk, Settings::value(Settings::AuthToken).toStdString(), usn, 1024, false);

                    if (usn >= chunk.updateCount)
                        break;

                    percent = (int)((double)(100*(double)usn/(double)chunk.updateCount));
                    emit progress(percent);

                    if (!chunk.notebooks.empty())
                        emit notebooksSynced(QVector<Notebook>::fromStdVector(chunk.notebooks));

                    emit progress(percent);
                    if (cancelled)
                        break;

                    if (!chunk.tags.empty())
                        emit tagsSynced(QVector<Tag>::fromStdVector(chunk.tags));

                    emit progress(percent);
                    if (cancelled)
                        break;

                    if (!chunk.resources.empty())
                        emit resourcesSynced(QVector<Resource>::fromStdVector(chunk.resources));

                    emit progress(percent);
                    if (cancelled)
                        break;

                    if (!chunk.notes.empty())
                        emit notesSynced(QVector<Note>::fromStdVector(chunk.notes));

                    emit progress(percent);
                    if (cancelled)
                        break;

                    qDebug() << Q_FUNC_INFO
                             << "NB:" << chunk.notebooks.size()
                             << "T:" << chunk.tags.size()
                             << "R:" << chunk.resources.size()
                             << "N:" << chunk.notes.size()
                             << "USN:" << chunk.chunkHighUSN;

                    usn = chunk.chunkHighUSN;
                    Settings::setValue(Settings::ServerUSN, QString::number(usn));
                    if (usn >= chunk.updateCount)
                        break;
                }
                break;
            } catch (EDAMUserException& e) {
                if (e.errorCode == 9)
                    qDebug() << "### TODO: REAUTH NEEDED";
                emit failed(e.what());
            }
        }
    } catch(TException& e) {
        qDebug() << Q_FUNC_INFO << "?" << e.what();
        emit failed(QString::fromStdString(e.what()));
    }

    syncing = false;
    emit activeChanged();
    if (!cancelled)
        emit finished();
}

void Synchronizer::fetchImpl(const evernote::edam::Note& note)
{
    if (fetching)
        return;

    qDebug() << Q_FUNC_INFO;

    fetching = true;
    cancelled = false;
    emit started();
    emit activeChanged();

    try {
        init(false);
        evernote::edam::Note copy(note);
        client->getNoteContent(copy.content, Settings::value(Settings::AuthToken).toStdString(), copy.guid);

        if (!copy.content.empty())
            emit noteFetched(copy);

        for (uint i = 0; !cancelled && i < copy.resources.size(); ++i) {
            Resource res = copy.resources.at(i);
            client->getResource(res, Settings::value(Settings::AuthToken).toStdString(), res.guid, true, false, false, false);
            emit resourceFetched(res);
        }

    } catch (TException& e) {
        qDebug() << Q_FUNC_INFO << "?" << e.what();
        emit failed(QString::fromStdString(e.what()));
    }

    fetching = false;
    emit activeChanged();
    if (!cancelled)
        emit finished();
}

void Synchronizer::init(bool force)
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
        client = new NoteStoreClient(boost::shared_ptr<TProtocol>(new TBinaryProtocol(transport)));
    }

    if (!transport->isOpen())
        transport->open();
}
