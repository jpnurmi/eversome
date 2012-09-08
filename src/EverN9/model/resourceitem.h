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
    Q_PROPERTY(Type type READ type CONSTANT)
    Q_PROPERTY(QString guid READ guid CONSTANT)
    Q_PROPERTY(QString mime READ mime CONSTANT)
    Q_PROPERTY(QString filePath READ filePath NOTIFY filePathChanged)
    Q_PROPERTY(QString thumbnail READ thumbnail NOTIFY thumbnailChanged)
    Q_PROPERTY(int usn READ usn CONSTANT)
    Q_ENUMS(Type)

public:
    explicit ResourceItem(evernote::edam::Resource resource = evernote::edam::Resource(), QObject* parent = 0);
    virtual ~ResourceItem();

    evernote::edam::Resource resource() const;

    enum Type { Unknown, Image, Audio, Document, Text };
    Type type() const;

    QString guid() const;
    QString mime() const;
    QString filePath(bool checkExists = true) const;
    QString thumbnail(bool checkExists = true) const;
    int usn() const;

    void update();

signals:
    void filePathChanged();
    void thumbnailChanged();

private:
    bool m_empty;
    evernote::edam::Resource m_resource;
};

Q_DECLARE_METATYPE(ResourceItem*)
Q_DECLARE_METATYPE(QList<ResourceItem*>)

QDebug operator<<(QDebug debug, const ResourceItem* item);

#endif // RESOURCEITEM_H
