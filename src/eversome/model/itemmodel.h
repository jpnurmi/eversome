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
#ifndef ITEMMODEL_H
#define ITEMMODEL_H

#include <QHash>
#include <QMetaType>
#include <QtAlgorithms>
#include <QAbstractListModel>

class ItemModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(int count READ rowCount NOTIFY countChanged)

public:
    explicit ItemModel(QObject* parent = 0);
    virtual ~ItemModel();

    Q_INVOKABLE QObject* at(int index) const;

    int rowCount(const QModelIndex& parent = QModelIndex()) const;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;

    bool contains(const QString& guid) const;

    template <typename T>
    QList<T> items() const;

    template <typename T>
    T get(const QString& guid) const;

    template <typename T>
    bool add(T item);

    template <typename T>
    bool add(const QList<T>& items);

    template <typename T>
    bool replace(T item);

    template <typename T>
    bool remove(T item);

    template <typename LessThan>
    void sort(LessThan lessThan);

public slots:
    void clear();

signals:
    void countChanged();
    void added(QObject* item);
    void added(const QObjectList& items);
    void removed(QObject* item);

private:
    QList<QObject*> m_items;
    QHash<QString, QObject*> m_guids;
};

#include "itemmodel_p.h"

Q_DECLARE_METATYPE(ItemModel*)

static bool namePropertyLessThan(const QObject* left, const QObject* right)
{
    return left->property("name").toString().localeAwareCompare(right->property("name").toString()) < 0;
}

static bool titlePropertyLessThan(const QObject* left, const QObject* right)
{
    return left->property("title").toString().localeAwareCompare(right->property("title").toString()) < 0;
}

#endif // ITEMMODEL_H
