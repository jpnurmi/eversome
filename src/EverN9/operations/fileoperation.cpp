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
    bool succeed = false;
    QFile file(m_filePath);
    QFileInfo info(file);

    switch (mode())
    {
        case ReadFile:
            if (file.open(QFile::ReadOnly))
                m_data = file.readAll();
            break;
        case WriteFile:
            if (QDir().mkpath(info.absolutePath())) {
                if (file.open(QFile::WriteOnly | QFile::Truncate))
                    succeed = file.write(m_data) > 0;

                if (succeed) {
                    QString thumbnail = info.baseName().replace('.', '-') + "-thumb.png";
                    QStringList args = QStringList() << "-geometry" << "128x128" << info.fileName()+"[0]" << thumbnail;
                    qDebug() << Q_FUNC_INFO << "/usr/bin/convert" << args;

                    // TODO: split as a separate operation
                    QProcess process;
                    process.setWorkingDirectory(info.absolutePath());
                    process.start("/usr/bin/convert", args);
                    if (!process.waitForFinished())
                        emit error(OperationError::toString(process.error()));

                    emit written(m_filePath);
                }
            }
            break;
        default:
            qWarning() << Q_FUNC_INFO << "unknown mode" << mode();
            break;
    }

    if (file.error() != QFile::NoError) {
        qDebug() << Q_FUNC_INFO << m_filePath << file.error();
        emit error(OperationError::toString(file.error()));
    }
}
