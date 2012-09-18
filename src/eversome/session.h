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
#ifndef SESSION_H
#define SESSION_H

#include <SignOn/AuthSession>
#include <SignOn/SessionData>
#include <SignOn/Identity>
#include <SignOn/Error>
#include <QUrl>

class Account;
using namespace SignOn;

class Session : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QUrl url READ url NOTIFY established)
    Q_PROPERTY(QString userName READ userName NOTIFY established)
    Q_PROPERTY(QString authToken READ authToken NOTIFY established)

public:
    explicit Session(const QString& host, QObject* parent = 0);
    virtual ~Session();

    QUrl url() const;
    QString userName() const;
    QString authToken() const;

public slots:
    void establish(int credentialsId);

signals:
    void established();
    void error(const QString& error);

private slots:
    void onError(const SignOn::Error& error);
    void onResponse(const SignOn::SessionData& data);
    void onStateChanged(AuthSession::AuthSessionState state, const QString& message);

private:
    QString m_host;
    SignOn::SessionData m_data;
    SignOn::Identity* m_identity;
    SignOn::AuthSession* m_session;
};

#endif // SESSION_H
