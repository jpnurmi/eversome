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
#ifndef RESOURCEITEM_H
#define RESOURCEITEM_H

#include <QDebug>
#include <QObject>
#include <QMetaType>
#include <Types_types.h>

class ResourceItem : public QObject
{
    Q_OBJECT
    Q_PROPERTY(Type type READ type NOTIFY dataChanged)
    Q_PROPERTY(QString guid READ guid NOTIFY dataChanged)
    Q_PROPERTY(QString mime READ mime NOTIFY dataChanged)
    Q_PROPERTY(QByteArray hash READ hash NOTIFY dataChanged)
    Q_PROPERTY(bool attachment READ isAttachment NOTIFY dataChanged)
    Q_PROPERTY(QString fileName READ fileName NOTIFY dataChanged)
    Q_PROPERTY(QString fileSize READ fileSize NOTIFY filePathChanged)
    Q_PROPERTY(QString filePath READ filePath NOTIFY filePathChanged)
    Q_PROPERTY(QString thumbnail READ thumbnail NOTIFY thumbnailChanged)
    Q_PROPERTY(int usn READ usn NOTIFY dataChanged)
    Q_ENUMS(Type)

public:
    explicit ResourceItem(evernote::edam::Resource resource = evernote::edam::Resource(), QObject* parent = 0);
    virtual ~ResourceItem();

    Q_INVOKABLE evernote::edam::Resource data() const;
    void setData(const evernote::edam::Resource& data);

    enum Type { Unknown, Image, Audio, Document, Text };
    Type type() const;

    QString guid() const;
    QString mime() const;
    QByteArray hash() const;
    bool isAttachment() const;
    QString fileName() const;
    QString fileSize() const;
    QString filePath(bool checkExists = true) const;
    QString thumbnail(bool checkExists = true) const;
    int usn() const;

signals:
    void dataChanged();
    void filePathChanged();
    void thumbnailChanged();

private:
    evernote::edam::Resource m_resource;

    friend class Manager;
};

Q_DECLARE_METATYPE(ResourceItem*)
Q_DECLARE_METATYPE(QList<ResourceItem*>)

QDebug operator<<(QDebug debug, const ResourceItem* item);

#endif // RESOURCEITEM_H
