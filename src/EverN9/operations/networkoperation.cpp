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
#include "operationerror.h"
#include <thrift/transport/THttpClient.h>
#include <thrift/transport/TTransportException.h>
#include <thrift/protocol/TProtocolException.h>
#include <thrift/protocol/TBinaryProtocol.h>
#include <Errors_types.h>

using namespace boost;
using namespace apache;
using namespace evernote;

NetworkOperation::NetworkOperation(Mode mode) : Operation(mode)
{
}

NetworkOperation::~NetworkOperation()
{
}

bool NetworkOperation::isValid() const
{
    return m_url.isValid() && !m_token.isEmpty();
}

QUrl NetworkOperation::url() const
{
    return m_url;
}

void NetworkOperation::setUrl(const QUrl& url)
{
    m_url = url;
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
    try {
        std::string host = m_url.host().toStdString();
        std::string path = m_url.path().toStdString();
        shared_ptr<thrift::transport::TTransport> transport(new thrift::transport::THttpClient(host, m_url.port(), path));
        shared_ptr<thrift::protocol::TProtocol> protocol(new thrift::protocol::TBinaryProtocol(transport));
        transport->open();
        operate(protocol);
    } catch (edam::EDAMUserException& e) {
        emit error(this, OperationError::toString(e.errorCode));
    } catch (edam::EDAMSystemException& e) {
        emit error(this, OperationError::toString(e.errorCode));
    } catch (edam::EDAMNotFoundException& e) {
        emit error(this, OperationError::toString(OperationError::UnknownOperation));
    } catch (thrift::transport::TTransportException& e) {
        emit error(this, OperationError::toString(e.getType()));
    } catch (thrift::protocol::TProtocolException& e) {
        emit error(this, OperationError::toString(e.getType()));
    } catch (thrift::TException& e) {
        qDebug() << Q_FUNC_INFO << "unknown error:" << e.what();
        emit error(this, OperationError::toString(OperationError::UnknownError));
    }
}
