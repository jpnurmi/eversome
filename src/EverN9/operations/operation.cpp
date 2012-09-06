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

#include "operation.h"
#include "manager.h"
#include <thrift/transport/THttpClient.h>
#include <thrift/protocol/TBinaryProtocol.h>
#include <Errors_types.h>
#include <QMetaEnum>
#include <QtDebug>

using namespace boost;
using namespace apache;
using namespace evernote;

Operation::Operation(Operation::Mode mode)
    : m_port(-1), m_mode(mode)
{
    qRegisterMetaType<Operation*>();
}

Operation::~Operation()
{
}

Operation::Mode Operation::mode() const
{
    return m_mode;
}

bool Operation::isValid() const
{
    return m_port != -1 && !m_host.isEmpty() && !m_path.isEmpty() && !m_token.isEmpty();
}

QString Operation::host() const
{
    return m_host;
}

void Operation::setHost(const QString& host)
{
    m_host = host;
}

int Operation::port() const
{
    return m_port;
}

void Operation::setPort(int port)
{
    m_port = port;
}

QString Operation::path() const
{
    return m_path;
}

void Operation::setPath(const QString& path)
{
    m_path = path;
}

QString Operation::authToken() const
{
    return m_token;
}

void Operation::setAuthToken(const QString& token)
{
    m_token = token;
}

void Operation::run()
{
    if (!isValid()) {
        qCritical() << Q_FUNC_INFO << "INVALID:" << this;
        return;
    }

    emit started(this);

    QString err;
    try {
        shared_ptr<thrift::transport::TTransport> transport(new thrift::transport::THttpClient(m_host.toStdString(), m_port, m_path.toStdString()));
        shared_ptr<thrift::protocol::TProtocol> protocol(new thrift::protocol::TBinaryProtocol(transport));
        transport->open();
        operate(protocol);
    } catch (edam::EDAMUserException& e) {
        err = Manager::errorString(e.errorCode);
    } catch (edam::EDAMSystemException& e) {
        err = Manager::errorString(e.errorCode);
    } catch (edam::EDAMNotFoundException& e) {
        err = Manager::errorString(-1); // TODO
    } catch (thrift::TException& e) {
        err = QString::fromUtf8(e.what());
    }

    if (!err.isEmpty()) {
        qDebug() << Q_FUNC_INFO << "ERROR:" << err << this;
        emit error(this, err);
    }

    emit finished(this);
}

QDebug operator<<(QDebug debug, const Operation* operation)
{
    if (!operation)
        return debug << "Operation(0x0) ";

    const QMetaObject* metaObject = operation->metaObject();

    debug.nospace() << metaObject->className() << '(' << (void*) operation;
    if (!operation->objectName().isEmpty())
        debug << ", name = " << operation->objectName();

    QMetaEnum enumerator = metaObject->enumerator(metaObject->indexOfEnumerator("Mode"));
    debug << ", mode = " << enumerator.valueToKey(operation->mode())
          << ", valid = " << operation->isValid() << ')';
    return debug.space();
}
