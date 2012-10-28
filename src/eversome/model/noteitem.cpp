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
#include <QDesktopServices>
#include <QFileInfo>
#include <QDir>

NoteItem::NoteItem(evernote::edam::Note note, QObject* parent)
    : QObject(parent), m_unread(true), m_note(note)
{
    m_resources = new ItemModel(this);
    m_tags = new ItemModel(this);
    m_tags->setSortProperty("title");
}

NoteItem::~NoteItem()
{
}

evernote::edam::Note NoteItem::data() const
{
    return m_note;
}

void NoteItem::setData(const evernote::edam::Note& data)
{
    m_note = data;
    emit dataChanged();
}

QString NoteItem::title() const
{
    return QString::fromStdString(m_note.title);
}

void NoteItem::setTitle(const QString& title)
{
    std::string str = title.toStdString();
    if (m_note.title != str) {
        m_note.title = str;
        emit dataChanged();
    }
}

QString NoteItem::guid() const
{
    return QString::fromStdString(m_note.guid);
}

QString NoteItem::filePath(bool checkExists) const
{
    QDir dir(QDesktopServices::storageLocation(QDesktopServices::DataLocation));
    QFileInfo file(dir.filePath(guid() + ".xml"));
    if (checkExists && (!file.exists() || file.size() == 0))
        return QString();
    return file.filePath();
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

QByteArray NoteItem::content() const
{
    return QByteArray(m_note.content.c_str(), m_note.content.size());
}

bool NoteItem::isUnread() const
{
    return m_unread;
}

void NoteItem::setUnread(bool unread)
{
    if (m_unread != unread) {
        m_unread = unread;
        emit unreadChanged();
    }
}

QDebug operator<<(QDebug debug, const NoteItem* item)
{
    if (!item)
        return debug << "NoteItem(0x0) ";
    debug.nospace() << item->metaObject()->className()
                    << '(' << (void*) item
                    << ", title = " << item->title()
                    << ", filePath = " << item->filePath(false)
                    << ", usn = " << item->usn() << ')';
    return debug.space();
}
