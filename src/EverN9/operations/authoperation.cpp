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

#include "authoperation.h"
#include "operationerror.h"
#include <UserStore_constants.h>
#include <UserStore.h>

using namespace boost;
using namespace apache;
using namespace evernote;

AuthOperation::AuthOperation(const QString& username, const QString& password, const QString& key, const QString& secret) :
    NetworkOperation(Auth), m_username(username), m_password(password), m_key(key), m_secret(secret)
{
}

AuthOperation::~AuthOperation()
{
}

QString AuthOperation::username() const
{
    return m_username;
}

QString AuthOperation::password() const
{
    return m_password;
}

edam::AuthenticationResult AuthOperation::result() const
{
    return m_result;
}

void AuthOperation::operate(shared_ptr<thrift::protocol::TProtocol> protocol)
{
    if (mode() != Auth) {
        qWarning() << Q_FUNC_INFO << "unknown mode" << mode();
        return;
    }

    edam::UserStoreClient client(protocol);
    if (!client.checkVersion("EverN9/0.0.3; MeeGo/Harmattan 1.2", // TODO: hardcoded values
                             edam::g_UserStore_constants.EDAM_VERSION_MAJOR,
                             edam::g_UserStore_constants.EDAM_VERSION_MINOR))
        emit error(OperationError::toString(OperationError::TooOldProtocol));
    else
        client.authenticate(m_result, m_username.toStdString(), m_password.toStdString(), m_key.toStdString(), m_secret.toStdString());
}
