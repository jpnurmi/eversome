#include "searchitem.h"

SearchItem::SearchItem(evernote::edam::SavedSearch search, QObject* parent)
    : QObject(parent), m_search(search)
{
}

SearchItem::~SearchItem()
{
}

evernote::edam::SavedSearch SearchItem::search() const
{
    return m_search;
}

QString SearchItem::guid() const
{
    return QString::fromStdString(m_search.guid);
}

QString SearchItem::name() const
{
    return QString::fromStdString(m_search.name);
}

QString SearchItem::query() const
{
    return QString::fromStdString(m_search.query);
}
