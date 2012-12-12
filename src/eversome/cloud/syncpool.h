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
#ifndef SYNCPOOL_H
#define SYNCPOOL_H

#include "abstractpool.h"
#include <QDateTime>
#include <Types_types.h>

class Session;
class NetworkOperation;

class SyncPool : public AbstractPool
{
    Q_OBJECT
    Q_PROPERTY(int usn READ usn NOTIFY usnChanged)
    Q_PROPERTY(QDateTime currentTime READ currentTime NOTIFY currentTimeChanged)

public:
    explicit SyncPool(Session* session);
    virtual ~SyncPool();

    int usn() const;
    QDateTime currentTime() const;

public slots:
    void sync();

signals:
    void usnChanged();
    void currentTimeChanged();

    void synced(const QVector<evernote::edam::Notebook>& notebooks,
                const QVector<evernote::edam::Resource>& resources,
                const QVector<evernote::edam::SavedSearch>& searches,
                const QVector<evernote::edam::Note>& notes,
                const QVector<evernote::edam::Tag>& tags);

    void expunged(const QVector<std::string>& notebooks,
                  const QVector<std::string>& searches,
                  const QVector<std::string>& notes,
                  const QVector<std::string>& tags);

private slots:
    void setUsn(int usn);
    void setCurrentTime(const QDateTime& time);
};

#endif // SYNCPOOL_H
