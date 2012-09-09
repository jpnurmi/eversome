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

FileOperation::FileOperation(Mode mode, const QString& filePath, const QByteArray& data) :
    Operation(mode), m_data(data), m_filePath(filePath)
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
                qDebug() << "FileOperation::operate()" << this << m_filePath << fileError;
                if (fileError != QFile::NoError)
                    emit error(OperationError::toString(fileError));
                else
                    emit read(m_filePath, m_data);
            }
            break;
        case WriteFile:
            if (QDir().mkpath(info.absolutePath())) {
                if (file.open(QFile::WriteOnly | QFile::Truncate))
                    file.write(m_data);

                QFile::FileError fileError = file.error();
                qDebug() << "FileOperation::operate()" << this << m_filePath << fileError;
                if (fileError != QFile::NoError)
                    emit error(OperationError::toString(fileError));
                else
                    emit written(m_filePath);
            }
            break;
        case GenerateThumbnail:
            if (QDir().mkpath(info.absolutePath())) {
                QString thumbnail = info.baseName().replace('.', '-') + "-thumb.png";
                QStringList args = QStringList() << "-geometry" << "128x128" << info.fileName()+"[0]" << thumbnail;

                QProcess process;
                process.setWorkingDirectory(info.absolutePath());
                process.start("/usr/bin/convert", args);

                if (!process.waitForFinished())
                    emit error(OperationError::toString(process.error()));
                else
                    emit generated(QDir(info.absolutePath()).filePath(thumbnail));
                qDebug() << "FileOperation::operate()" << this << m_filePath << process.error();
                break;
            }
        default:
            Q_ASSERT(false);
            break;
    }
}
