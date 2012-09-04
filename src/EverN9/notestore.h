#ifndef NOTESTORE_H
#define NOTESTORE_H

#include <QObject>
#include "edam/NoteStore.h"
#include "edam/Types_types.h"

class NoteStore : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool active READ isActive NOTIFY activeChanged)

public:
    explicit NoteStore(QObject *parent = 0);
    virtual ~NoteStore();

    bool isActive() const;

public slots:
    void sync();
    void cancel();
    void fetch(const evernote::edam::Note& note);

signals:
    void started();
    void progress(int percent);
    void error(const QString& error);
    void finished();
    void activeChanged();

    void synced(const QVector<evernote::edam::Notebook>& notebooks,
                const QVector<evernote::edam::Resource>& resources,
                const QVector<evernote::edam::SavedSearch>& searches,
                const QVector<evernote::edam::Note>& notes,
                const QVector<evernote::edam::Tag>& tags);

    void resourceFetched(const evernote::edam::Resource& resource);
    void noteFetched(const evernote::edam::Note& note);

private slots:
    void syncImpl();
    void fetchImpl(const evernote::edam::Note& note);
    void init(bool force);

private:
    volatile bool syncing;
    volatile bool fetching;
    volatile bool cancelled;
    evernote::edam::NoteStoreClient* client;
    boost::shared_ptr<apache::thrift::transport::TTransport> transport;
};

#endif // NOTESTORE_H
