#ifndef SYNCHRONIZER_H
#define SYNCHRONIZER_H

#include <QObject>
#include "edam/NoteStore.h"
#include "edam/Types_types.h"

class Synchronizer : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool active READ isActive NOTIFY activeChanged)

public:
    explicit Synchronizer(QObject *parent = 0);
    ~Synchronizer();

    bool isActive() const;

public slots:
    void sync();
    void cancel();

signals:
    void started();
    void progress(int percent);
    void failed(const QString& error);
    void finished();
    void activeChanged();

    void notebooksSynced(const QVector<evernote::edam::Notebook>& notebooks);
    void resourcesSynced(const QVector<evernote::edam::Resource>& resources);
    void notesSynced(const QVector<evernote::edam::Note>& notes);
    void tagsSynced(const QVector<evernote::edam::Tag>& tags);

private slots:
    void syncImpl();
    void init(bool force);

private:
    volatile bool syncing;
    volatile bool cancelled;
    evernote::edam::NoteStoreClient* client;
    boost::shared_ptr<apache::thrift::transport::TTransport> transport;
};

#endif // SYNCHRONIZER_H
