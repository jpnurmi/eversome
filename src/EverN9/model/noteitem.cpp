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
#include "noteitem.h"

NoteItem::NoteItem(evernote::edam::Note note, QObject* parent)
    : QObject(parent), m_note(note)
{
    m_tags = new ItemModel(this);
    m_resources = new ItemModel(this);
}

NoteItem::~NoteItem()
{
}

evernote::edam::Note NoteItem::note() const
{
    return m_note;
}

QString NoteItem::guid() const
{
    return QString::fromStdString(m_note.guid);
}

QString NoteItem::title() const
{
    return QString::fromStdString(m_note.title);
}

QString NoteItem::content() const
{
    return QString::fromStdString(m_note.content);
}

QDateTime NoteItem::created() const
{
    return QDateTime::fromMSecsSinceEpoch(m_note.created);
}

QDateTime NoteItem::updated() const
{
    return QDateTime::fromMSecsSinceEpoch(m_note.updated);
}

QDateTime NoteItem::deleted() const
{
    return QDateTime::fromMSecsSinceEpoch(m_note.deleted);
}

bool NoteItem::isActive() const
{
    return m_note.active;
}

bool NoteItem::isEmpty() const
{
    return m_note.content.empty();
}

ItemModel* NoteItem::tags() const
{
    return m_tags;
}

ItemModel* NoteItem::resources() const
{
    return m_resources;
}

int NoteItem::usn() const
{
    return m_note.updateSequenceNum;
}

void NoteItem::setContent(const std::string& content)
{
    if (m_note.content != content) {
        m_note.content = content;
        emit isEmptyChanged();
        emit contentChanged();
    }
}

QDebug operator<<(QDebug debug, const NoteItem* item)
{
    if (!item)
        return debug << "NoteItem(0x0) ";
    debug.nospace() << item->metaObject()->className()
                    << '(' << (void*) item
                    << ", title = " << item->title()
                    << ", usn = " << item->usn() << ')';
    return debug.space();
}
