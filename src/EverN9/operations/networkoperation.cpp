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

#include "networkoperation.h"
#include "manager.h"
#include <thrift/transport/THttpClient.h>
#include <thrift/protocol/TBinaryProtocol.h>
#include <Errors_types.h>

using namespace boost;
using namespace apache;
using namespace evernote;

NetworkOperation::NetworkOperation(Mode mode) : Operation(mode), m_port(-1)
{
}

NetworkOperation::~NetworkOperation()
{
}

bool NetworkOperation::isValid() const
{
    return m_port != -1 && !m_host.isEmpty() && !m_path.isEmpty();
}

QString NetworkOperation::host() const
{
    return m_host;
}

void NetworkOperation::setHost(const QString& host)
{
    m_host = host;
}

int NetworkOperation::port() const
{
    return m_port;
}

void NetworkOperation::setPort(int port)
{
    m_port = port;
}

QString NetworkOperation::path() const
{
    return m_path;
}

void NetworkOperation::setPath(const QString& path)
{
    m_path = path;
}

QString NetworkOperation::authToken() const
{
    return m_token;
}

void NetworkOperation::setAuthToken(const QString& token)
{
    m_token = token;
}

void NetworkOperation::operate()
{
    QString err;
    try {
        shared_ptr<thrift::transport::TTransport> transport(new thrift::transport::THttpClient(m_host.toStdString(), m_port, m_path.toStdString()));
        shared_ptr<thrift::protocol::TProtocol> protocol(new thrift::protocol::TBinaryProtocol(transport));
        transport->open();
        operate(protocol);
    } catch (edam::EDAMUserException& e) {
        // TODO: refactor error string handling
        err = Manager::errorString(e.errorCode);
    } catch (edam::EDAMSystemException& e) {
        // TODO: refactor error string handling
        err = Manager::errorString(e.errorCode);
    } catch (edam::EDAMNotFoundException& e) {
        // TODO: refactor error string handling
        err = Manager::errorString(-1); // TODO
    } catch (thrift::TException& e) {
        err = QString::fromUtf8(e.what());
    }

    if (!err.isEmpty()) {
        qDebug() << Q_FUNC_INFO << "ERROR:" << err << this;
        emit error(this, err);
    }
}
