#ifndef USERSTORE_H
#define USERSTORE_H

#include <QObject>
#include "edam/UserStore.h"

class UserStore : public QObject
{
    Q_OBJECT

public:
    explicit UserStore(QObject* parent = 0);
    virtual ~UserStore();

    Q_INVOKABLE bool hasCredentials() const;

public slots:
    void authenticate(const QString& username = QString(), const QString& password = QString());

signals:
    void succeed();
    void failed(const QString& error);

private slots:
    void authImpl(const QString& username, const QString& password);

private:
    evernote::edam::UserStoreClient* client;
    boost::shared_ptr<apache::thrift::transport::TTransport> transport;
};

#endif // USERSTORE_H
