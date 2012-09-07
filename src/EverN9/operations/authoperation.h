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
#ifndef AUTHOPERATION_H
#define AUTHOPERATION_H

#include "networkoperation.h"
#include <UserStore_types.h>

class AuthOperation : public NetworkOperation
{
    Q_OBJECT

public:
    AuthOperation(const QString& username, const QString& password, const QString& key, const QString& secret);
    ~AuthOperation();

    QString username() const;
    QString password() const;

    evernote::edam::AuthenticationResult result() const;

protected:
    void operate(boost::shared_ptr<apache::thrift::protocol::TProtocol> protocol);

private:
    QString m_username;
    QString m_password;
    QString m_key;
    QString m_secret;
    evernote::edam::AuthenticationResult m_result;
};

#endif // AUTHOPERATION_H
