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
#include "noteitem.h"

TagItem::TagItem(evernote::edam::Tag tag, QObject* parent)
    : QObject(parent), m_tag(tag)
{
    m_notes = new ItemModel(this);
    m_notes->setSortProperty("title");

    connect(m_notes, SIGNAL(countChanged()), this, SIGNAL(unreadChanged()));
    connect(m_notes, SIGNAL(added(QObject*)), this, SLOT(onNoteAdded(QObject*)));
    connect(m_notes, SIGNAL(added(QObjectList)), this, SLOT(onNotesAdded(QObjectList)));
    connect(m_notes, SIGNAL(removed(QObject*)), this, SLOT(onNoteRemoved(QObject*)));
}

TagItem::~TagItem()
{
}

evernote::edam::Tag TagItem::data() const
{
    return m_tag;
}

void TagItem::setData(const evernote::edam::Tag& data)
{
    m_tag = data;
    emit dataChanged();
}

QString TagItem::name() const
{
    return QString::fromStdString(m_tag.name);
}

void TagItem::setName(const QString& name)
{
    std::string str = name.toStdString();
    if (m_tag.name != str) {
        m_tag.name = str;
        emit dataChanged();
    }
}

QString TagItem::guid() const
{
    return QString::fromStdString(m_tag.guid);
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

bool TagItem::isUnread() const
{
    foreach (NoteItem* note, m_notes->items<NoteItem*>()) {
        if (note->isUnread())
            return true;
    }
    return false;
}

void TagItem::setUnread(bool unread)
{
    foreach (NoteItem* note, m_notes->items<NoteItem*>())
        note->setUnread(unread);
}

void TagItem::onNoteAdded(QObject* note)
{
    connect(note, SIGNAL(unreadChanged()), this, SIGNAL(unreadChanged()));
}

void TagItem::onNotesAdded(const QObjectList& notes)
{
    foreach (QObject* note, notes)
        connect(note, SIGNAL(unreadChanged()), this, SIGNAL(unreadChanged()));
}

void TagItem::onNoteRemoved(QObject* note)
{
    disconnect(note, SIGNAL(unreadChanged()), this, SIGNAL(unreadChanged()));
}

QDebug operator<<(QDebug debug, const TagItem* item)
{
    if (!item)
        return debug << "TagItem(0x0) ";
    debug.nospace() << item->metaObject()->className()
                    << '(' << (void*) item
                    << ", name = " << item->name()
                    << ", usn = " << item->usn() << ')';
    return debug.space();
}
