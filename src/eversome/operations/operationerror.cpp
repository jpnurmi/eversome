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
#include "operationerror.h"
#include <QCoreApplication>
#include <Errors_constants.h>

using namespace evernote;
using namespace apache::thrift::protocol;
using namespace apache::thrift::transport;

static const char* other_errors[] =
{
    QT_TRANSLATE_NOOP("OperationError", "An unknown error occurred"),
    QT_TRANSLATE_NOOP("OperationError", "An unknown operation requested"),
    QT_TRANSLATE_NOOP("OperationError", "The application uses too old protocol version: %1.%2")
};

QString OperationError::toString(OtherError error)
{
    return QCoreApplication::translate("OperationError", other_errors[error]);
}

static const char* file_errors[] =
{
    QT_TRANSLATE_NOOP("OperationError", "An error occurred while reading from a file"),
    QT_TRANSLATE_NOOP("OperationError", "An error occurred while writing to a file"),
    QT_TRANSLATE_NOOP("OperationError", "A fatal file error occurred"),
    QT_TRANSLATE_NOOP("OperationError", "A file resource error"),
    QT_TRANSLATE_NOOP("OperationError", "A file could not be opened"),
    QT_TRANSLATE_NOOP("OperationError", "A file  operation was aborted"),
    QT_TRANSLATE_NOOP("OperationError", "A timeout occurred"),
    QT_TRANSLATE_NOOP("OperationError", "An unspecified file error occurred"),
    QT_TRANSLATE_NOOP("OperationError", "A file could not be removed"),
    QT_TRANSLATE_NOOP("OperationError", "A file could not be renamed"),
    QT_TRANSLATE_NOOP("OperationError", "The position in a file could not be changed"),
    QT_TRANSLATE_NOOP("OperationError", "A file could not be resized"),
    QT_TRANSLATE_NOOP("OperationError", "A file could not be accessed"),
    QT_TRANSLATE_NOOP("OperationError", "A file could not be copied")
};

QString OperationError::toString(QFile::FileError error)
{
    if (error <= QFile::NoError || error > QFile::CopyError)
        return toString(UnknownError);
    return QCoreApplication::translate("OperationError", file_errors[error-1]);
}

static const char* process_errors[] =
{
    QT_TRANSLATE_NOOP("OperationError", "A process failed to start"),
    QT_TRANSLATE_NOOP("OperationError", "A process crashed"),
    QT_TRANSLATE_NOOP("OperationError", "A process timed out"),
    QT_TRANSLATE_NOOP("OperationError", "An error occurred while writing to a process"),
    QT_TRANSLATE_NOOP("OperationError", "An error occurred while reading from a process"),
};

QString OperationError::toString(QProcess::ProcessError error)
{
    if (error < QProcess::FailedToStart || error >= QProcess::UnknownError)
        return toString(UnknownError);
    return QCoreApplication::translate("OperationError", process_errors[error]);
}

static const char* edam_errors[] =
{
    QT_TRANSLATE_NOOP("OperationError", "The format of the request data was incorrect"),
    QT_TRANSLATE_NOOP("OperationError", "Not permitted to perform action"),
    QT_TRANSLATE_NOOP("OperationError", "Unexpected problem with the service"),
    QT_TRANSLATE_NOOP("OperationError", "A required parameter/field was absent"),
    QT_TRANSLATE_NOOP("OperationError", "Operation denied due to data model limit"),
    QT_TRANSLATE_NOOP("OperationError", "Operation denied due to user storage limit"),
    QT_TRANSLATE_NOOP("OperationError", "Incorrect username and/or password"),
    QT_TRANSLATE_NOOP("OperationError", "Authentication token expired"),
    QT_TRANSLATE_NOOP("OperationError", "Change denied due to data model conflict"),
    QT_TRANSLATE_NOOP("OperationError", "Content of submitted note was malformed"),
    QT_TRANSLATE_NOOP("OperationError", "Service shard with account data is temporarily down"),
    QT_TRANSLATE_NOOP("OperationError", "Operation denied due to data model limit (too short)"),
    QT_TRANSLATE_NOOP("OperationError", "Operation denied due to data model limit (too long)"),
    QT_TRANSLATE_NOOP("OperationError", "Operation denied due to data model limit (too few)"),
    QT_TRANSLATE_NOOP("OperationError", "Operation denied due to data model limit (too many)"),
    QT_TRANSLATE_NOOP("OperationError", "Operation denied because it is currently unsupported")
};

QString OperationError::toString(edam::EDAMErrorCode error)
{
    if (error <= edam::UNKNOWN || error > edam::UNSUPPORTED_OPERATION)
        return toString(UnknownError);
    return QCoreApplication::translate("OperationError", edam_errors[error-1]);
}

static const char* protocol_errors[] =
{
    QT_TRANSLATE_NOOP("OperationError", "Invalid protocol data"),
    QT_TRANSLATE_NOOP("OperationError", "Negative protocol data size"),
    QT_TRANSLATE_NOOP("OperationError", "Protocol data exceeded the size limit"),
    QT_TRANSLATE_NOOP("OperationError", "Invalid protocol version"),
    QT_TRANSLATE_NOOP("OperationError", "Protocol not implemented")
};

QString OperationError::toString(TProtocolException::TProtocolExceptionType error)
{
    if (error <= TProtocolException::UNKNOWN || error  > TProtocolException::NOT_IMPLEMENTED)
        return toString(UnknownError);
    return QCoreApplication::translate("OperationError", protocol_errors[error-1]);
}

static const char* transport_errors[] =
{
    QT_TRANSLATE_NOOP("OperationError", "Transport was not open"),
    QT_TRANSLATE_NOOP("OperationError", "Transport timed out"),
    QT_TRANSLATE_NOOP("OperationError", "End of file while transporting"),
    QT_TRANSLATE_NOOP("OperationError", "Transport was interrupted"),
    QT_TRANSLATE_NOOP("OperationError", "Invalid transport arguments"),
    QT_TRANSLATE_NOOP("OperationError", "Corrupted transport data"),
    QT_TRANSLATE_NOOP("OperationError", "Internal transport error")
};

QString OperationError::toString(TTransportException::TTransportExceptionType error)
{
    if (error <= TTransportException::UNKNOWN || error  > TTransportException::INTERNAL_ERROR)
        return toString(UnknownError);
    return QCoreApplication::translate("OperationError", transport_errors[error-1]);
}
