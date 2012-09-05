#include "noteitem.h"

NoteItem::NoteItem(evernote::edam::Note note, QObject* parent)
    : QObject(parent), m_note(note)
{
    m_tags = new ItemModel(this);
    m_resources = new ItemModel(this);
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

bool NoteItem::isEmpty() const
{
    return m_note.content.empty();
}

ItemModel* NoteItem::tags() const
{
    return m_tags;
}

ItemModel* NoteItem::resources() const
{
    return m_resources;
}

void NoteItem::setContent(const std::string& content)
{
    if (m_note.content != content) {
        m_note.content = content;
        emit isEmptyChanged();
        emit contentChanged();
    }
}
