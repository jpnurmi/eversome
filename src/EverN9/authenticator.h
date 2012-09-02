#ifndef AUTHENTICATOR_H
#define AUTHENTICATOR_H

#include <QObject>
#include "edam/UserStore.h"

class Authenticator : public QObject
{
    Q_OBJECT

public:
    explicit Authenticator(QObject *parent = 0);
    ~Authenticator();

public slots:
    void auth(const QString& username, const QString& password);

signals:
    void succeed();
    void failed(const QString& error);

private slots:
    void authImpl(const QString& username, const QString& password);

private:
    evernote::edam::UserStoreClient* client;
    boost::shared_ptr<apache::thrift::transport::TTransport> transport;
};

#endif // AUTHENTICATOR_H
