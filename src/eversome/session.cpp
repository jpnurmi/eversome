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

#include "session.h"
#include <QtDebug>

static const int EDAM_PORT = 80;

Session::Session(const QString& host, QObject* parent) :
    QObject(parent), m_host(host), m_identity(0), m_session(0)
{
}

Session::~Session()
{
}

QString Session::userName() const
{
    return m_data.getProperty("UserName").toString();
}

QString Session::authToken() const
{
    return m_data.getProperty("SecretKey").toString();
}

QUrl Session::url(const QString& path) const
{
    QUrl url;
    url.setScheme("http");
    url.setHost(m_host);
    url.setPort(EDAM_PORT);
    QString uid = m_data.getProperty("Uid").toString();
    url.setPath(QString("/shard/%1/%2").arg(uid).arg(path));
    return url;
}

void Session::establish(int credentialsId)
{
    m_identity = SignOn::Identity::existingIdentity(credentialsId, this);

    // TODO:
    if (!m_identity) {
        SignOn::IdentityInfo info;
        info.setCaption("Evernote");
        info.setMethod("evernote", QStringList() << "AuthLogin");
        m_identity = SignOn::Identity::newIdentity(info, this);
    }

    m_session = m_identity->createSession("evernote");

    qDebug() << Q_FUNC_INFO << m_identity << credentialsId << m_session;

    connect(m_session, SIGNAL(error(SignOn::Error)),
                 this, SLOT(onError(SignOn::Error)));
    connect(m_session, SIGNAL(response(SignOn::SessionData)),
                 this, SLOT(onResponse(SignOn::SessionData)));
    connect(m_session, SIGNAL(stateChanged(AuthSession::AuthSessionState,QString)),
                 this, SLOT(onStateChanged(AuthSession::AuthSessionState,QString)));

    m_session->process(SignOn::SessionData(), "AuthLogin");
}

void Session::onError(const SignOn::Error& err)
{
    qDebug() << Q_FUNC_INFO << err.type() << err.message();
    emit error(err.message());
}

void Session::onResponse(const SignOn::SessionData& data)
{
    qDebug() << Q_FUNC_INFO << data.propertyNames();
    m_data = data;
    emit established();
}

void Session::onStateChanged(SignOn::AuthSession::AuthSessionState state, const QString& message)
{
    qDebug() << Q_FUNC_INFO << state << message;
}
