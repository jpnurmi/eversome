#include "notebookitem.h"

NotebookItem::NotebookItem(evernote::edam::Notebook notebook, QObject* parent)
    : QObject(parent), m_notebook(notebook)
{
    m_notes = new ItemModel(this);
}

NotebookItem::~NotebookItem()
{
}

ItemModel* NotebookItem::notes() const
{
    return m_notes;
}

evernote::edam::Notebook NotebookItem::notebook() const
{
    return m_notebook;
}

QString NotebookItem::guid() const
{
    return QString::fromStdString(m_notebook.guid);
}

QString NotebookItem::name() const
{
    return QString::fromStdString(m_notebook.name);
}

bool NotebookItem::isDefault() const
{
    return m_notebook.defaultNotebook;
}

bool NotebookItem::isPublished() const
{
    return m_notebook.published;
}

QDateTime NotebookItem::created() const
{
    return QDateTime::fromMSecsSinceEpoch(m_notebook.serviceCreated);
}

QDateTime NotebookItem::updated() const
{
    return QDateTime::fromMSecsSinceEpoch(m_notebook.serviceUpdated);
}
