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
#ifndef ABSTRACTOPERATION_H
#define ABSTRACTOPERATION_H

#include <QDebug>
#include <QObject>
#include <QString>
#include <QRunnable>

class AbstractOperation : public QObject, public QRunnable
{
    Q_OBJECT

public:
    AbstractOperation();
    ~AbstractOperation();

    virtual bool isValid() const = 0;
    virtual void run();

signals:
    void started();
    void finished();
    void error(const QString& error);

protected:
    virtual void operate() = 0;
};

QDebug operator<<(QDebug debug, const AbstractOperation* operation);

#endif // ABSTRACTOPERATION_H
