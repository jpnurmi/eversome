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
#include "tagitem.h"

TagItem::TagItem(evernote::edam::Tag tag, QObject* parent)
    : QObject(parent), m_tag(tag)
{
    m_notes = new ItemModel(this);
}

TagItem::~TagItem()
{
}

evernote::edam::Tag TagItem::tag() const
{
    return m_tag;
}

QString TagItem::guid() const
{
    return QString::fromStdString(m_tag.guid);
}

QString TagItem::name() const
{
    return QString::fromStdString(m_tag.name);
}

QString TagItem::parentGuid() const
{
    return QString::fromStdString(m_tag.parentGuid);
}

ItemModel* TagItem::notes() const
{
    return m_notes;
}

int TagItem::usn() const
{
    return m_tag.updateSequenceNum;
}

QDebug operator<<(QDebug debug, const TagItem* item)
{
    if (!item)
        return debug << "TagItem(0x0) ";
    debug.nospace() << item->metaObject()->className()
                    << '(' << (void*) item
                    << ", name = " << item->name() << ')';
    return debug.space();
}
