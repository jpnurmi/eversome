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
#include <QDesktopServices>
#include <QFileInfo>
#include <QFile>
#include <QHash>
#include <QDir>

typedef QHash<QString, QString> StringHash;
Q_GLOBAL_STATIC_WITH_INITIALIZER(StringHash, file_extensions, {
    x->insert("application/msword", "doc");
    x->insert("application/vnd.openxmlformats-officedocument.wordprocessingml.document", "docx");
    x->insert("application/pdf", "pdf");
    x->insert("audio/amr", "amr");
    x->insert("audio/mpeg", "mp3");
    x->insert("audio/wav", "wav");
    x->insert("audio/vnd.wave", "wav");
    x->insert("image/bmp", "bmp");
    x->insert("image/gif", "gif");
    x->insert("image/jpg", "jpg");
    x->insert("image/jpeg", "jpg");
    x->insert("image/pjpeg", "jpg");
    x->insert("image/png", "png");
    x->insert("text/plain", "txt");
    x->insert("text/html", "html");
})

typedef QHash<QString, ResourceItem::Type> TypeHash;
Q_GLOBAL_STATIC_WITH_INITIALIZER(TypeHash, file_types, {
    x->insert("application", ResourceItem::Document);
    x->insert("audio", ResourceItem::Audio);
    x->insert("image", ResourceItem::Image);
    x->insert("text", ResourceItem::Text);
})

ResourceItem::ResourceItem(evernote::edam::Resource resource, QObject* parent)
    : QObject(parent), m_empty(false), m_resource(resource)
{
}

ResourceItem::~ResourceItem()
{
}

evernote::edam::Resource ResourceItem::resource() const
{
    return m_resource;
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

static QString dataFilePath(const QString& fileName)
{
    QDir dir(QDesktopServices::storageLocation(QDesktopServices::DataLocation));
    return dir.absoluteFilePath(fileName);
}

QString ResourceItem::filePath(bool checkExists) const
{
    QString ext = file_extensions()->value(QString::fromStdString(m_resource.mime));
    QFileInfo file(dataFilePath(guid() + "." + ext));
    if (checkExists && (m_empty || !file.exists() || file.size() == 0))
        return QString();
    return file.filePath();
}

QString ResourceItem::thumbnail(bool checkExists) const
{
    QFileInfo file(dataFilePath(guid() + "-thumb.png"));
    if (checkExists && (m_empty || !file.exists() || file.size() == 0))
        return QString();
    return file.filePath();
}

int ResourceItem::usn() const
{
    return m_resource.updateSequenceNum;
}

void ResourceItem::update()
{
    m_empty = true;
    emit filePathChanged();
    emit thumbnailChanged();

    m_empty = false;
    emit filePathChanged();
    emit thumbnailChanged();
}

QDebug operator<<(QDebug debug, const ResourceItem* item)
{
    if (!item)
        return debug << "ResourceItem(0x0) ";
    debug.nospace() << item->metaObject()->className()
                    << '(' << (void*) item
                    << ", filePath = " << item->filePath(false) << ')';
    return debug.space();
}
