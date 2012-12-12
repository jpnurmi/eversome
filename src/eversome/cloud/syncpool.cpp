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

#include "syncpool.h"
#include "session.h"
#include "syncoperation.h"
#include <QSettings>
#include <QtDebug>

using namespace evernote;

Q_DECLARE_METATYPE(QVector<std::string>)

SyncPool::SyncPool(Session* session) : AbstractPool(session)
{
    connect(session, SIGNAL(established()), SLOT(sync()));

    qRegisterMetaType<QVector<std::string> >();
}

SyncPool::~SyncPool()
{
}

int SyncPool::usn() const
{
    return QSettings().value("usn").toInt();
}

void SyncPool::setUsn(int val)
{
    int old = usn();
    if (old != val) {
        QSettings().setValue("usn", val);
        emit usnChanged();
    }
}

QDateTime SyncPool::currentTime() const
{
    return QSettings().value("time").toDateTime();
}

void SyncPool::setCurrentTime(const QDateTime& val)
{
    QDateTime old = currentTime();
    if (old != val) {
        QSettings().setValue("time", val);
        emit currentTimeChanged();
    }
}

void SyncPool::sync()
{
    SyncOperation* operation = new SyncOperation(usn());
    connect(operation, SIGNAL(usnChanged(int)), this, SLOT(setUsn(int)));
    connect(operation, SIGNAL(currentTimeChanged(QDateTime)), this, SLOT(setCurrentTime(QDateTime)));
    connect(operation, SIGNAL(synced(QVector<evernote::edam::Notebook>,
                                     QVector<evernote::edam::Resource>,
                                     QVector<evernote::edam::SavedSearch>,
                                     QVector<evernote::edam::Note>,
                                     QVector<evernote::edam::Tag>)),
                 this, SIGNAL(synced(QVector<evernote::edam::Notebook>,
                                     QVector<evernote::edam::Resource>,
                                     QVector<evernote::edam::SavedSearch>,
                                     QVector<evernote::edam::Note>,
                                     QVector<evernote::edam::Tag>)));
    connect(operation, SIGNAL(expunged(QVector<std::string>,
                                       QVector<std::string>,
                                       QVector<std::string>,
                                       QVector<std::string>)),
                 this, SIGNAL(expunged(QVector<std::string>,
                                       QVector<std::string>,
                                       QVector<std::string>,
                                       QVector<std::string>)));
    startOperation(operation, "notestore");
}
