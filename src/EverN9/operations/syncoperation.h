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

#include "baseoperation.h"
#include <Types_types.h>
#include <QVector>

class SyncOperation : public BaseOperation
{
    Q_OBJECT

public:
    SyncOperation(int usn);
    ~SyncOperation();

    int usn() const;
    QVector<evernote::edam::Notebook> notebooks() const;
    QVector<evernote::edam::Resource> resources() const;
    QVector<evernote::edam::SavedSearch> searches() const;
    QVector<evernote::edam::Note> notes() const;
    QVector<evernote::edam::Tag> tags() const;

    void operate(boost::shared_ptr<apache::thrift::protocol::TProtocol> protocol);

private:
    int m_usn;
    QVector<evernote::edam::Notebook> m_notebooks;
    QVector<evernote::edam::Resource> m_resources;
    QVector<evernote::edam::SavedSearch> m_searches;
    QVector<evernote::edam::Note> m_notes;
    QVector<evernote::edam::Tag> m_tags;
};

#endif // SYNCOPERATION_H
