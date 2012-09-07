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

#include <QObject>

class Operation;

class ResourceWriter : public QObject
{
    Q_OBJECT

public:
    explicit ResourceWriter(QObject* parent = 0);
    virtual ~ResourceWriter();

    void write(const QString& filePath, const QByteArray& data);

signals:
    void activityChanged();
    void error(const QString& error);
    void written(const QString& filePath);

private slots:
    void onOperationStarted(Operation* operation);
    void onOperationFinished(Operation* operation);
    void onOperationError(Operation* operation, const QString& error);
};

#endif // RESOURCEWRITER_H
