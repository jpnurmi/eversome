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
#ifndef NOTEITEM_H
#define NOTEITEM_H

#include <QDebug>
#include <QObject>
#include <QDateTime>
#include <QMetaType>
#include <Types_types.h>
#include "itemmodel.h"

class NoteItem : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString title READ title WRITE setTitle NOTIFY dataChanged)
    Q_PROPERTY(QString guid READ guid NOTIFY dataChanged)
    Q_PROPERTY(QString filePath READ filePath NOTIFY filePathChanged)
    Q_PROPERTY(QDateTime created READ created NOTIFY dataChanged)
    Q_PROPERTY(QDateTime updated READ updated NOTIFY dataChanged)
    Q_PROPERTY(QDateTime deleted READ deleted NOTIFY dataChanged)
    Q_PROPERTY(bool isActive READ isActive NOTIFY dataChanged)
    Q_PROPERTY(ItemModel* tags READ tags CONSTANT)
    Q_PROPERTY(ItemModel* resources READ resources CONSTANT)
    Q_PROPERTY(int usn READ usn NOTIFY dataChanged)
    Q_PROPERTY(QString html READ html NOTIFY dataChanged)
    Q_PROPERTY(bool unread READ isUnread WRITE setUnread NOTIFY unreadChanged)

public:
    explicit NoteItem(evernote::edam::Note note = evernote::edam::Note(), QObject* parent = 0);
    virtual ~NoteItem();

    Q_INVOKABLE evernote::edam::Note data() const;
    void setData(const evernote::edam::Note& data);

    QString title() const;
    void setTitle(const QString& title);

    QString guid() const;
    QString filePath(bool checkExists = true) const;
    QDateTime created() const;
    QDateTime updated() const;
    QDateTime deleted() const;
    bool isActive() const;
    ItemModel* tags() const;
    ItemModel* resources() const;
    int usn() const;
    QString html() const;

    bool isUnread() const;
    void setUnread(bool unread);

signals:
    void dataChanged();
    void filePathChanged();
    void unreadChanged();

private:
    bool m_unread;
    ItemModel* m_tags;
    ItemModel* m_resources;
    evernote::edam::Note m_note;

    friend class Manager;
};

Q_DECLARE_METATYPE(NoteItem*)
Q_DECLARE_METATYPE(QList<NoteItem*>)

QDebug operator<<(QDebug debug, const NoteItem* item);

#endif // NOTEITEM_H
