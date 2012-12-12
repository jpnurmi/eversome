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
#ifndef SYNCOPERATION_H
#define SYNCOPERATION_H

#include "networkoperation.h"
#include <Types_types.h>
#include <QDateTime>

class SyncOperation : public NetworkOperation
{
    Q_OBJECT

public:
    SyncOperation(int usn);
    ~SyncOperation();

signals:
    void usnChanged(int usn);
    void currentTimeChanged(const QDateTime& time);

    void synced(const QVector<evernote::edam::Notebook>& notebooks,
                const QVector<evernote::edam::Resource>& resources,
                const QVector<evernote::edam::SavedSearch>& searches,
                const QVector<evernote::edam::Note>& notes,
                const QVector<evernote::edam::Tag>& tags);

    void expunged(const QVector<std::string>& notebooks,
                  const QVector<std::string>& searches,
                  const QVector<std::string>& notes,
                  const QVector<std::string>& tags);

protected:
    void operate(boost::shared_ptr<apache::thrift::protocol::TProtocol> protocol);

private:
    int m_usn;
};

#endif // SYNCOPERATION_H
