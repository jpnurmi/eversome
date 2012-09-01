#include "noteitem.h"
#include "tagmodel.h"
#include "tagitem.h"

NoteItem::NoteItem(evernote::edam::Note note, QObject* parent)
    : QObject(parent), m_note(note)
{
    qRegisterMetaType<NoteItem*>();
}

NoteItem::~NoteItem()
{
}

evernote::edam::Note NoteItem::note() const
{
    return m_note;
}

QString NoteItem::guid() const
{
    return QString::fromStdString(m_note.guid);
}

QString NoteItem::title() const
{
    return QString::fromStdString(m_note.title);
}

QString NoteItem::content() const
{
    return QString::fromStdString(m_note.content);
}

QDateTime NoteItem::created() const
{
    return QDateTime::fromMSecsSinceEpoch(m_note.created);
}

QDateTime NoteItem::updated() const
{
    return QDateTime::fromMSecsSinceEpoch(m_note.updated);
}

QDateTime NoteItem::deleted() const
{
    return QDateTime::fromMSecsSinceEpoch(m_note.deleted);
}

bool NoteItem::isActive() const
{
    return m_note.active;
}

QStringList NoteItem::tags() const
{
    QStringList res;
    for (uint i = 0; i < m_note.tagGuids.size(); ++i) {
        QString guid = QString::fromStdString(m_note.tagGuids.at(i));
        TagItem* tag = TagModel::instance()->get(guid);
        if (tag)
            res += tag->name();
    }
    return res;
}
