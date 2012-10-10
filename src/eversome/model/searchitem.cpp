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
#include "noteitem.h"

SearchItem::SearchItem(evernote::edam::SavedSearch search, QObject* parent)
    : QObject(parent), m_search(search)
{
    m_notes = new ItemModel(this);
    m_notes->setSortProperty("title");

    connect(m_notes, SIGNAL(countChanged()), this, SIGNAL(unreadChanged()));
    connect(m_notes, SIGNAL(added(QObject*)), this, SLOT(onNoteAdded(QObject*)));
    connect(m_notes, SIGNAL(added(QObjectList)), this, SLOT(onNotesAdded(QObjectList)));
    connect(m_notes, SIGNAL(removed(QObject*)), this, SLOT(onNoteRemoved(QObject*)));
}

SearchItem::~SearchItem()
{
}

evernote::edam::SavedSearch SearchItem::data() const
{
    return m_search;
}

void SearchItem::setData(const evernote::edam::SavedSearch& data)
{
    m_search = data;
    emit dataChanged();
}

QString SearchItem::name() const
{
    return QString::fromStdString(m_search.name);
}

void SearchItem::setName(const QString& name)
{
    std::string str = name.toStdString();
    if (m_search.name != str) {
        m_search.name = str;
        emit dataChanged();
    }
}

QString SearchItem::guid() const
{
    return QString::fromStdString(m_search.guid);
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

bool SearchItem::isUnread() const
{
    foreach (NoteItem* note, m_notes->items<NoteItem*>()) {
        if (note->isUnread())
            return true;
    }
    return false;
}

void SearchItem::setUnread(bool unread)
{
    foreach (NoteItem* note, m_notes->items<NoteItem*>())
        note->setUnread(unread);
}

void SearchItem::onNoteAdded(QObject* note)
{
    connect(note, SIGNAL(unreadChanged()), this, SIGNAL(unreadChanged()));
}

void SearchItem::onNotesAdded(const QObjectList& notes)
{
    foreach (QObject* note, notes)
        connect(note, SIGNAL(unreadChanged()), this, SIGNAL(unreadChanged()));
}

void SearchItem::onNoteRemoved(QObject* note)
{
    disconnect(note, SIGNAL(unreadChanged()), this, SIGNAL(unreadChanged()));
}

QDebug operator<<(QDebug debug, const SearchItem* item)
{
    if (!item)
        return debug << "SearchItem(0x0) ";
    debug.nospace() << item->metaObject()->className()
                    << '(' << (void*) item
                    << ", name = " << item->name()
                    << ", usn = " << item->usn() << ')';
    return debug.space();
}
