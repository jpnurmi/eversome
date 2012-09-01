#include "tagitem.h"

TagItem::TagItem(evernote::edam::Tag tag, QObject* parent)
    : QObject(parent), m_tag(tag)
{
    m_notes = new NoteModel(this);
    qRegisterMetaType<TagItem*>();
}

TagItem::~TagItem()
{
}

NoteModel* TagItem::notes() const
{
    return m_notes;
}

evernote::edam::Tag TagItem::tag() const
{
    return m_tag;
}

QString TagItem::guid() const
{
    return QString::fromStdString(m_tag.guid);
}

QString TagItem::name() const
{
    return QString::fromStdString(m_tag.name);
}

QString TagItem::parentGuid() const
{
    return QString::fromStdString(m_tag.parentGuid);
}
