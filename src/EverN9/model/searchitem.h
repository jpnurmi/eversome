#ifndef SEARCHITEM_H
#define SEARCHITEM_H

#include <QObject>
#include <QMetaType>
#include "edam/Types_types.h"
#include "itemmodel.h"

class SearchItem : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString guid READ guid CONSTANT)
    Q_PROPERTY(QString name READ name CONSTANT)
    Q_PROPERTY(QString query READ query CONSTANT)
    Q_PROPERTY(ItemModel* notes READ notes CONSTANT)

public:
    explicit SearchItem(evernote::edam::SavedSearch search = evernote::edam::SavedSearch(), QObject* parent = 0);
    virtual ~SearchItem();

    Q_INVOKABLE evernote::edam::SavedSearch search() const;

    QString guid() const;
    QString name() const;
    QString query() const;
    ItemModel* notes() const;

private:
    ItemModel* m_notes;
    evernote::edam::SavedSearch m_search;
};

Q_DECLARE_METATYPE(SearchItem*)
Q_DECLARE_METATYPE(QList<SearchItem*>)

#endif // SEARCHITEM_H
