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

#include "abstractpool.h"
#include "abstractoperation.h"
#include <QThreadPool>
#include <QtDebug>

AbstractPool::AbstractPool(QObject* parent) : QObject(parent)
{
    // thread(s) do not expire -> one thread per pool
    QThreadPool::globalInstance()->setExpiryTimeout(-1);
    QThreadPool::globalInstance()->setMaxThreadCount(1);
}

AbstractPool::~AbstractPool()
{
}

void AbstractPool::startOperation(AbstractOperation* operation)
{
    connect(operation, SIGNAL(started()), this, SIGNAL(activityChanged()));
    connect(operation, SIGNAL(finished()), this, SIGNAL(activityChanged()));
    connect(operation, SIGNAL(error(QString)), this, SIGNAL(error(QString)));
    qDebug().nospace() << metaObject()->className() << "::startOperation(): " << operation;
    QThreadPool::globalInstance()->start(operation);
}
