#ifndef RESOURCEITEM_H
#define RESOURCEITEM_H

#include <QObject>
#include <QMetaType>
#include "edam/Types_types.h"

class ResourceItem : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString guid READ guid CONSTANT)
    Q_PROPERTY(QString mime READ mime CONSTANT)
    Q_PROPERTY(QString filePath READ filePath NOTIFY filePathChanged)
    Q_PROPERTY(QString thumbnail READ thumbnail NOTIFY thumbnailChanged)
    Q_PROPERTY(Type type READ type CONSTANT)
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
    bool isEmpty() const;

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

#endif // RESOURCEITEM_H
