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
#ifndef FILEOPERATION_H
#define FILEOPERATION_H

#include "operation.h"

class FileOperation : public Operation
{
    Q_OBJECT

public:
    FileOperation(Mode mode, const QString& guid, const QString& filePath, const QByteArray& data = QByteArray());
    ~FileOperation();

    bool isValid() const;

signals:
    void read(const QString& guid, const QString& filePath, const QByteArray& data);
    void wrote(const QString& guid, const QString& filePath);
    void generated(const QString& guid, const QString& filePath);

protected:
    void operate();

private:
    QString m_guid;
    QByteArray m_data;
    QString m_filePath;
};

#endif // FILEOPERATION_H
