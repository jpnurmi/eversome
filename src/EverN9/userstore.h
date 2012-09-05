/*
* Copyright (C) 2012 J-P Nurmi <jpnurmi@gmail.com>
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*/
#ifndef USERSTORE_H
#define USERSTORE_H

#include <QUrl>
#include <QObject>
#include <QDateTime>
#include "edam/UserStore.h"

class UserStore : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString authToken READ authToken NOTIFY loggedIn)
    Q_PROPERTY(QUrl notesUrl READ notesUrl NOTIFY loggedIn)
    Q_PROPERTY(QDateTime currentTime READ currentTime NOTIFY loggedIn)
    Q_PROPERTY(QDateTime expiration READ expiration NOTIFY loggedIn)
    Q_PROPERTY(bool isActive READ isActive NOTIFY isActiveChanged)

public:
    explicit UserStore(QObject* parent = 0);
    virtual ~UserStore();

    QString authToken() const;
    QUrl notesUrl() const;

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
    QUrl notes;
    QDateTime time;
    QDateTime expires;
    volatile bool active;
};

#endif // USERSTORE_H
