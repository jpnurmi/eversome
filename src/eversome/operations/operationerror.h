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
#ifndef OPERATIONERROR_H
#define OPERATIONERROR_H

#include <QFile>
#include <QString>
#include <QProcess>
#include <Errors_types.h>
#include <thrift/protocol/TProtocolException.h>
#include <thrift/transport/TTransportException.h>

struct OperationError
{
    enum OtherError { UnknownError, UnknownOperation, TooOldProtocol };

    static QString toString(OtherError error);
    static QString toString(QFile::FileError error);
    static QString toString(QProcess::ProcessError error);
    static QString toString(evernote::edam::EDAMErrorCode error);
    static QString toString(apache::thrift::protocol::TProtocolException::TProtocolExceptionType error);
    static QString toString(apache::thrift::transport::TTransportException::TTransportExceptionType error);
};

#endif // OPERATIONERROR_H
