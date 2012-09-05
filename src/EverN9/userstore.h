#ifndef USERSTORE_H
#define USERSTORE_H

#include <QObject>
#include <QDateTime>
#include "edam/UserStore.h"

class UserStore : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString authToken READ authToken NOTIFY loggedIn)
    Q_PROPERTY(QString notesUrl READ notesUrl NOTIFY loggedIn)
    Q_PROPERTY(QDateTime currentTime READ currentTime NOTIFY loggedIn)
    Q_PROPERTY(QDateTime expiration READ expiration NOTIFY loggedIn)
    Q_PROPERTY(bool isActive READ isActive NOTIFY isActiveChanged)

public:
    explicit UserStore(QObject* parent = 0);
    virtual ~UserStore();

    QString authToken() const;
    QString notesUrl() const;

    QDateTime currentTime() const;
    QDateTime expiration() const;

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
    QString token;
    QString notes;
    QDateTime time;
    QDateTime expires;
    volatile bool active;
};

#endif // USERSTORE_H
