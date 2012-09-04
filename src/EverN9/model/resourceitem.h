#ifndef RESOURCEITEM_H
#define RESOURCEITEM_H

#include <QObject>
#include <QMetaType>
#include "edam/Types_types.h"

class ResourceItem : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString guid READ guid CONSTANT)
    Q_PROPERTY(QString filePath READ filePath CONSTANT)

public:
    explicit ResourceItem(evernote::edam::Resource resource = evernote::edam::Resource(), QObject* parent = 0);
    virtual ~ResourceItem();

    evernote::edam::Resource resource() const;

    QString guid() const;
    QString filePath() const;

private:
    evernote::edam::Resource m_resource;
};

Q_DECLARE_METATYPE(ResourceItem*)
Q_DECLARE_METATYPE(QList<ResourceItem*>)

#endif // RESOURCEITEM_H
