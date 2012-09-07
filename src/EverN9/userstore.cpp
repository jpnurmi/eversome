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
//#define QT_NO_DEBUG_OUTPUT

#include "userstore.h"
#include "settings.h"
#include "manager.h"
#include "authoperation.h"
#include <QThreadPool>
#include <QVariant>
#include <QDebug>

using namespace evernote;

UserStore::UserStore(QObject *parent) : QObject(parent), active(false)
{
}

UserStore::~UserStore()
{
}

QString UserStore::authToken() const
{
    return token;
}

QUrl UserStore::notesUrl() const
{
    return notes;
}

QDateTime UserStore::currentTime() const
{
    return time;
}

QDateTime UserStore::expiration() const
{
    return expires;
}

bool UserStore::hasCredentials() const
{
    return !Settings::value(Settings::Username).isEmpty() &&
           !Settings::value(Settings::Password).isEmpty();
}

void UserStore::login(const QString& username, const QString& password, bool remember)
{
    QString un = username.isEmpty() ? Settings::value(Settings::Username) : username;
    QString pw = password.isEmpty() ? Settings::value(Settings::Password) : password;
    QString key = Settings::value(Settings::ConsumerKey);
    QString secret = Settings::value(Settings::ConsumerSecret);

    AuthOperation* operation = new AuthOperation(un, pw, key, secret);
    connect(operation, SIGNAL(started(Operation*)),
                 this, SLOT(onOperationStarted(Operation*)), Qt::DirectConnection);
    connect(operation, SIGNAL(finished(Operation*)),
                 this, SLOT(onOperationFinished(Operation*)), Qt::DirectConnection);
    connect(operation, SIGNAL(error(Operation*,QString)),
                 this, SLOT(onOperationError(Operation*,QString)), Qt::DirectConnection);
    operation->setHost(Settings::value(Settings::Hostname));
    operation->setPort(Settings::value(Settings::ServerPort).toInt());
    operation->setPath("/edam/user");

    operation->setProperty("remember", remember);

    qDebug() << Q_FUNC_INFO << operation << username << QString(password.length(), 'x');
    QThreadPool::globalInstance()->start(operation);
}

void UserStore::logout()
{
    qDebug() << Q_FUNC_INFO;
    Settings::reset();
    emit loggedOut();
}

void UserStore::onOperationStarted(Operation* operation)
{
    qDebug() << Q_FUNC_INFO << operation;
    emit activityChanged();
}

void UserStore::onOperationFinished(Operation* operation)
{
    qDebug() << Q_FUNC_INFO << operation;

    AuthOperation* authOperation = qobject_cast<AuthOperation*>(operation);
    if (authOperation) {
        if (operation->property("remember").toBool()) {
            Settings::setValue(Settings::Username, authOperation->username());
            Settings::setValue(Settings::Password, authOperation->password());
        }

        edam::AuthenticationResult result = authOperation->result();
        token = QString::fromStdString(result.authenticationToken);
        notes = QUrl(QString::fromStdString(result.noteStoreUrl));
        time = QDateTime::fromMSecsSinceEpoch(result.currentTime);
        expires = QDateTime::fromMSecsSinceEpoch(result.expiration);

        emit loggedIn();
    }

    operation->deleteLater();

    emit activityChanged();
}

void UserStore::onOperationError(Operation* operation, const QString& error)
{
    qDebug() << Q_FUNC_INFO << operation << error;
}
