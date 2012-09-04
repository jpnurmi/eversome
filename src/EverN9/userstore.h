#ifndef USERSTORE_H
#define USERSTORE_H

#include <QObject>
#include "edam/UserStore.h"

class UserStore : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool active READ isActive NOTIFY activeChanged)

public:
    explicit UserStore(QObject* parent = 0);
    virtual ~UserStore();

    bool isActive() const;
    Q_INVOKABLE bool hasCredentials() const;

public slots:
    void login(const QString& username = QString(), const QString& password = QString(), bool remember = true);
    void logout();

signals:
    void loggedIn();
    void loggedOut();
    void error(const QString& error);
    void activeChanged();

private slots:
    void loginImpl(const QString& username, const QString& password, bool remember);
    void logoutImpl();

private:
    volatile bool loggingIn;
    volatile bool loggingOut;
    evernote::edam::UserStoreClient* client;
    boost::shared_ptr<apache::thrift::transport::TTransport> transport;
};

#endif // USERSTORE_H
