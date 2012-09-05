#ifndef NOTESTORE_H
#define NOTESTORE_H

#include <QObject>
#include "edam/NoteStore.h"
#include "edam/Types_types.h"

class UserStore;

class NoteStore : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool isActive READ isActive NOTIFY isActiveChanged)

public:
    explicit NoteStore(UserStore *userStore);
    virtual ~NoteStore();

    bool isActive() const;

public slots:
    void sync();
    void cancel();
    void fetch(const evernote::edam::Note& note);
    void search(const evernote::edam::SavedSearch& search);

signals:
    void started();
    void progress(int percent);
    void error(const QString& error);
    void finished();
    void isActiveChanged();

    void synced(const QVector<evernote::edam::Notebook>& notebooks,
                const QVector<evernote::edam::Resource>& resources,
                const QVector<evernote::edam::SavedSearch>& searches,
                const QVector<evernote::edam::Note>& notes,
                const QVector<evernote::edam::Tag>& tags);

    void resourceFetched(const evernote::edam::Resource& resource);
    void noteFetched(const evernote::edam::Note& note);

    void searched(const evernote::edam::SavedSearch& search, const QVector<evernote::edam::Note>& notes);

private slots:
    void syncImpl();
    void fetchImpl(const evernote::edam::Note& note);
    void searchImpl(const evernote::edam::SavedSearch& search);
    void init(bool force);

private:
    volatile bool syncing;
    volatile bool fetching;
    volatile bool searching;
    volatile bool cancelled;
    UserStore* userStore;
    evernote::edam::NoteStoreClient* client;
    boost::shared_ptr<apache::thrift::transport::TTransport> transport;
};

#endif // NOTESTORE_H
