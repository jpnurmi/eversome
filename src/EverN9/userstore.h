#ifndef USERSTORE_H
#define USERSTORE_H

#include <QObject>
#include "edam/UserStore.h"

class UserStore : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool isActive READ isActive NOTIFY isActiveChanged)

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
    void isActiveChanged();
    void error(const QString& error);

private slots:
    void loginImpl(const QString& username, const QString& password, bool remember);

private:
    volatile bool active;
};

#endif // USERSTORE_H
