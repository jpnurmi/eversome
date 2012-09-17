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
#include "notebookitem.h"
#include "noteitem.h"

NotebookItem::NotebookItem(evernote::edam::Notebook notebook, QObject* parent)
    : QObject(parent), m_notebook(notebook)
{
    m_notes = new ItemModel(this);
    connect(m_notes, SIGNAL(countChanged()), this, SIGNAL(unreadChanged()));
    connect(m_notes, SIGNAL(added(QObject*)), this, SLOT(onNoteAdded(QObject*)));
    connect(m_notes, SIGNAL(added(QObjectList)), this, SLOT(onNotesAdded(QObjectList)));
    connect(m_notes, SIGNAL(removed(QObject*)), this, SLOT(onNoteRemoved(QObject*)));
}

NotebookItem::~NotebookItem()
{
}

evernote::edam::Notebook NotebookItem::data() const
{
    return m_notebook;
}

void NotebookItem::setData(const evernote::edam::Notebook& data)
{
    m_notebook = data;
    emit dataChanged();
}

QString NotebookItem::guid() const
{
    return QString::fromStdString(m_notebook.guid);
}

QString NotebookItem::name() const
{
    return QString::fromStdString(m_notebook.name);
}

bool NotebookItem::isDefault() const
{
    return m_notebook.defaultNotebook;
}

bool NotebookItem::isPublished() const
{
    return m_notebook.published;
}

QDateTime NotebookItem::created() const
{
    return QDateTime::fromMSecsSinceEpoch(m_notebook.serviceCreated);
}

QDateTime NotebookItem::updated() const
{
    return QDateTime::fromMSecsSinceEpoch(m_notebook.serviceUpdated);
}

ItemModel* NotebookItem::notes() const
{
    return m_notes;
}

int NotebookItem::usn() const
{
    return m_notebook.updateSequenceNum;
}

bool NotebookItem::isUnread() const
{
    foreach (NoteItem* note, m_notes->items<NoteItem*>()) {
        if (note->isUnread())
            return true;
    }
    return false;
}

void NotebookItem::setUnread(bool unread)
{
    foreach (NoteItem* note, m_notes->items<NoteItem*>())
        note->setUnread(unread);
}

void NotebookItem::onNoteAdded(QObject* note)
{
    connect(note, SIGNAL(unreadChanged()), this, SIGNAL(unreadChanged()));
}

void NotebookItem::onNotesAdded(const QObjectList& notes)
{
    foreach (QObject* note, notes)
        connect(note, SIGNAL(unreadChanged()), this, SIGNAL(unreadChanged()));
}

void NotebookItem::onNoteRemoved(QObject* note)
{
    disconnect(note, SIGNAL(unreadChanged()), this, SIGNAL(unreadChanged()));
}

QDebug operator<<(QDebug debug, const NotebookItem* item)
{
    if (!item)
        return debug << "NotebookItem(0x0) ";
    debug.nospace() << item->metaObject()->className()
                    << '(' << (void*) item
                    << ", name = " << item->name()
                    << ", usn = " << item->usn() << ')';
    return debug.space();
}
