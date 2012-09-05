/*
* Copyright (C) 2012 J-P Nurmi <jpnurmi@gmail.com>
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*/
#ifndef SEARCHITEM_H
#define SEARCHITEM_H

#include <QObject>
#include <QMetaType>
#include <Types_types.h>
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
