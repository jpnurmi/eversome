#ifndef TAGITEM_H
#define TAGITEM_H

#include <QObject>
#include <QMetaType>
#include "edam/Types_types.h"
#include "itemmodel.h"

class TagItem : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString guid READ guid CONSTANT)
    Q_PROPERTY(QString name READ name CONSTANT)
    Q_PROPERTY(QString parentGuid READ parentGuid CONSTANT)
    Q_PROPERTY(ItemModel* notes READ notes CONSTANT)

public:
    explicit TagItem(evernote::edam::Tag tag = evernote::edam::Tag(), QObject* parent = 0);
    virtual ~TagItem();

    evernote::edam::Tag tag() const;

    QString guid() const;
    QString name() const;
    QString parentGuid() const;
    ItemModel* notes() const;

private:
    ItemModel* m_notes;
    evernote::edam::Tag m_tag;
};

Q_DECLARE_METATYPE(TagItem*)
Q_DECLARE_METATYPE(QList<TagItem*>)

#endif // TAGITEM_H
