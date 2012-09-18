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
#ifndef TAGITEM_H
#define TAGITEM_H

#include <QDebug>
#include <QObject>
#include <QMetaType>
#include <Types_types.h>
#include "itemmodel.h"

class TagItem : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString guid READ guid NOTIFY dataChanged)
    Q_PROPERTY(QString name READ name NOTIFY dataChanged)
    Q_PROPERTY(QString parentGuid READ parentGuid NOTIFY dataChanged)
    Q_PROPERTY(ItemModel* notes READ notes CONSTANT)
    Q_PROPERTY(int usn READ usn NOTIFY dataChanged)
    Q_PROPERTY(bool unread READ isUnread WRITE setUnread NOTIFY unreadChanged)

public:
    explicit TagItem(evernote::edam::Tag tag = evernote::edam::Tag(), QObject* parent = 0);
    virtual ~TagItem();

    Q_INVOKABLE evernote::edam::Tag data() const;
    void setData(const evernote::edam::Tag& data);

    QString guid() const;
    QString name() const;
    QString parentGuid() const;
    ItemModel* notes() const;
    int usn() const;

    bool isUnread() const;
    void setUnread(bool unread);

signals:
    void dataChanged();
    void unreadChanged();

private slots:
    void onNoteAdded(QObject* note);
    void onNotesAdded(const QObjectList& notes);
    void onNoteRemoved(QObject* note);

private:
    ItemModel* m_notes;
    evernote::edam::Tag m_tag;
};

Q_DECLARE_METATYPE(TagItem*)
Q_DECLARE_METATYPE(QList<TagItem*>)

QDebug operator<<(QDebug debug, const TagItem* item);

#endif // TAGITEM_H
