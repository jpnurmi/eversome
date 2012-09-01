#ifndef TAGITEM_H
#define TAGITEM_H

#include <QObject>
#include <QMetaType>
#include "edam/Types_types.h"

class TagItem : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString guid READ guid CONSTANT)
    Q_PROPERTY(QString name READ name CONSTANT)
    Q_PROPERTY(QString parentGuid READ parentGuid CONSTANT)

public:
    explicit TagItem(evernote::edam::Tag tag = evernote::edam::Tag(), QObject* parent = 0);
    virtual ~TagItem();

    evernote::edam::Tag tag() const;

    QString guid() const;
    QString name() const;
    QString parentGuid() const;

private:
    evernote::edam::Tag m_tag;
};

Q_DECLARE_METATYPE(TagItem*)

#endif // TAGITEM_H
