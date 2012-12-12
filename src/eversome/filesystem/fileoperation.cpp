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

#include "fileoperation.h"
#include "operationerror.h"
#include <QFileInfo>
#include <QProcess>
#include <QVariant>
#include <QFile>
#include <QDir>

FileOperation::FileOperation(Mode mode, const QString& guid, const QString& filePath, const QByteArray& data) :
    Operation(mode), m_guid(guid), m_data(data), m_filePath(filePath)
{
}

FileOperation::~FileOperation()
{
}

bool FileOperation::isValid() const
{
    return !m_filePath.isEmpty();
}

void FileOperation::operate()
{
    QFile file(m_filePath);
    QFileInfo info(file);

    switch (mode())
    {
        case ReadFile:
            if (file.open(QFile::ReadOnly)) {
                m_data = file.readAll();

                QFile::FileError fileError = file.error();
                qDebug() << "FileOperation::operate(): read..."
                         << (fileError == QFile::NoError ? "OK" : "FAIL!")
                         << qPrintable("("+info.fileName()+")");
                if (fileError != QFile::NoError)
                    emit error(OperationError::toString(fileError));
                else
                    emit read(m_guid, m_filePath, m_data);
            }
            break;
        case WriteFile:
            if (QDir().mkpath(info.absolutePath())) {
                if (file.open(QFile::WriteOnly | QFile::Truncate))
                    file.write(m_data);

                QFile::FileError fileError = file.error();
                qDebug() << "FileOperation::operate(): wrote..."
                         << (fileError == QFile::NoError ? "OK" : "FAIL!")
                         << qPrintable("("+info.fileName()+")");
                if (fileError != QFile::NoError)
                    emit error(OperationError::toString(fileError));
                else
                    emit wrote(m_guid, m_filePath);
            }
            break;
        default:
            Q_ASSERT(false);
            break;
    }
}
