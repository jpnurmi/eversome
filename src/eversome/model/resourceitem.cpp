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
#include "resourceitem.h"
#include <Limits_constants.h>
#include <QDesktopServices>
#include <QFileInfo>
#include <QFile>
#include <QHash>
#include <QDir>

typedef QHash<QString, QString> StringHash;
Q_GLOBAL_STATIC_WITH_INITIALIZER(StringHash, file_extensions, {
    x->insert("application/msword", ".doc");
    x->insert("application/vnd.openxmlformats-officedocument.wordprocessingml.document", ".docx");
    x->insert("application/pdf", ".pdf");
    x->insert("audio/amr", ".amr");
    x->insert("audio/mpeg", ".mp3");
    x->insert("audio/wav", ".wav");
    x->insert("audio/vnd.wave", ".wav");
    x->insert("image/bmp", ".bmp");
    x->insert("image/gif", ".gif");
    x->insert("image/jpg", ".jpg");
    x->insert("image/jpeg", ".jpg");
    x->insert("image/pjpeg", ".jpg");
    x->insert("image/png", ".png");
    x->insert("text/plain", ".txt");
    x->insert("text/html", ".html");
})

typedef QHash<QString, ResourceItem::Type> TypeHash;
Q_GLOBAL_STATIC_WITH_INITIALIZER(TypeHash, file_types, {
    x->insert("application", ResourceItem::Document);
    x->insert("audio", ResourceItem::Audio);
    x->insert("image", ResourceItem::Image);
    x->insert("text", ResourceItem::Text);
})

ResourceItem::ResourceItem(evernote::edam::Resource resource, QObject* parent)
    : QObject(parent), m_resource(resource)
{
}

ResourceItem::~ResourceItem()
{
}

evernote::edam::Resource ResourceItem::data() const
{
    return m_resource;
}

void ResourceItem::setData(const evernote::edam::Resource& data)
{
    m_resource = data;
    emit dataChanged();
}

ResourceItem::Type ResourceItem::type() const
{
    QString type = QString::fromStdString(m_resource.mime).section('/', 0, 0);
    return file_types()->value(type, Unknown);
}

QString ResourceItem::guid() const
{
    return QString::fromStdString(m_resource.guid);
}

QString ResourceItem::mime() const
{
    return QString::fromStdString(m_resource.mime);
}

QByteArray ResourceItem::hash() const
{
    return QByteArray(m_resource.data.bodyHash.c_str(), evernote::limits::g_Limits_constants.EDAM_HASH_LEN).toHex();
}

bool ResourceItem::isAttachment() const
{
    return m_resource.attributes.attachment;
}

QString ResourceItem::fileName() const
{
    if (!m_resource.attributes.fileName.empty())
        return QString::fromStdString(m_resource.attributes.fileName);
    return "noname" + file_extensions()->value(mime());
}

static QString dataFilePath(const QByteArray& hash, const QString& fileName)
{
    QDir dir(QDesktopServices::storageLocation(QDesktopServices::DataLocation));
    return dir.absoluteFilePath(hash + "/" + fileName);
}

QString ResourceItem::filePath(bool checkExists) const
{
    QFileInfo file(dataFilePath(hash(), fileName().isEmpty() ? guid() : fileName()));
    if (checkExists && (!file.exists() || file.size() == 0))
        return QString();
    return file.filePath();
}

QString ResourceItem::thumbnail(bool checkExists) const
{
    QFileInfo file(dataFilePath(hash(), "thumb.png"));
    if (checkExists && (!file.exists() || file.size() == 0))
        return QString();
    return file.filePath();
}

int ResourceItem::usn() const
{
    return m_resource.updateSequenceNum;
}

QDebug operator<<(QDebug debug, const ResourceItem* item)
{
    if (!item)
        return debug << "ResourceItem(0x0) ";
    debug.nospace() << item->metaObject()->className()
                    << '(' << (void*) item
                    << ", filePath = " << item->filePath(false)
                    << ", usn = " << item->usn() << ')';
    return debug.space();
}
