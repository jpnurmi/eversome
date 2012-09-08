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
#include "searchitem.h"

SearchItem::SearchItem(evernote::edam::SavedSearch search, QObject* parent)
    : QObject(parent), m_search(search)
{
    m_notes = new ItemModel(this);
}

SearchItem::~SearchItem()
{
}

evernote::edam::SavedSearch SearchItem::search() const
{
    return m_search;
}

QString SearchItem::guid() const
{
    return QString::fromStdString(m_search.guid);
}

QString SearchItem::name() const
{
    return QString::fromStdString(m_search.name);
}

QString SearchItem::query() const
{
    return QString::fromStdString(m_search.query);
}

ItemModel* SearchItem::notes() const
{
    return m_notes;
}

int SearchItem::usn() const
{
    return m_search.updateSequenceNum;
}

QDebug operator<<(QDebug debug, const SearchItem* item)
{
    if (!item)
        return debug << "SearchItem(0x0) ";
    debug.nospace() << item->metaObject()->className()
                    << '(' << (void*) item
                    << ", name = " << item->name() << ')';
    return debug.space();
}
