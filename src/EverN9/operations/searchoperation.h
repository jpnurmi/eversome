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
#ifndef SEARCHOPERATION_H
#define SEARCHOPERATION_H

#include "operation.h"
#include <Types_types.h>
#include <QVector>

class SearchOperation : public Operation
{
    Q_OBJECT

public:
    SearchOperation(const evernote::edam::SavedSearch& search);
    ~SearchOperation();

    evernote::edam::SavedSearch search() const;
    QVector<evernote::edam::Note> notes() const;

    void operate(boost::shared_ptr<apache::thrift::protocol::TProtocol> protocol);

private:
    evernote::edam::SavedSearch m_search;
    QVector<evernote::edam::Note> m_notes;
};

#endif // SEARCHOPERATION_H
