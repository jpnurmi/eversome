#ifndef TAGITEM_H
#define TAGITEM_H

#include <QObject>
#include <QMetaType>
#include "edam/Types_types.h"
#include "notemodel.h"

class TagItem : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString guid READ guid CONSTANT)
    Q_PROPERTY(QString name READ name CONSTANT)
    Q_PROPERTY(QString parentGuid READ parentGuid CONSTANT)
    Q_PROPERTY(NoteModel* notes READ notes CONSTANT)

public:
    explicit TagItem(evernote::edam::Tag tag = evernote::edam::Tag(), QObject* parent = 0);
    virtual ~TagItem();

    evernote::edam::Tag tag() const;

    QString guid() const;
    QString name() const;
    QString parentGuid() const;
    NoteModel* notes() const;

private:
    NoteModel* m_notes;
    evernote::edam::Tag m_tag;
};

Q_DECLARE_METATYPE(TagItem*)

#endif // TAGITEM_H