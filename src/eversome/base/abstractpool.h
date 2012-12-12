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
#ifndef ABSTRACTPOOL_H
#define ABSTRACTPOOL_H

#include <QObject>

class AbstractOperation;

class AbstractPool : public QObject
{
    Q_OBJECT

public:
    explicit AbstractPool(QObject* parent = 0);
    virtual ~AbstractPool();

    void startOperation(AbstractOperation* operation);

signals:
    void activityChanged();
    void error(const QString& error);
};

#endif // ABSTRACTPOOL_H
