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
#ifndef RESOURCEWRITER_H
#define RESOURCEWRITER_H

#include <QSet>
#include <QObject>
#include <QProcess>

class ResourceWriter : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool isWriting READ isWriting NOTIFY isWritingChanged)

public:
    explicit ResourceWriter(QObject* parent = 0);
    virtual ~ResourceWriter();

    bool isWriting() const;

    void write(const QString& filePath, const QByteArray& data);

signals:
    void isWritingChanged();
    void error(const QString& error);
    void written(const QString& filePath);

private slots:
    void writeImpl(const QString& filePath, const QByteArray& data);

private:
    QSet<QString> m_files;
};

#endif // RESOURCEWRITER_H
