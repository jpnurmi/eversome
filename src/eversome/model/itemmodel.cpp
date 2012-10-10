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
#include "itemmodel.h"
#include <QDebug>

class PropertySorter
{
public:
    PropertySorter(const QByteArray& property, Qt::SortOrder order)
        : property(property), order(order) { }

    bool operator() (const QObject* left, const QObject* right) const
    {
        QString l = left->property(property).toString();
        QString r = right->property(property).toString();
        if (order == Qt::DescendingOrder) qSwap(l, r);
        return l.localeAwareCompare(r) < 0;
    }

private:
    QByteArray property;
    Qt::SortOrder order;
};

ItemModel::ItemModel(QObject *parent) : QAbstractListModel(parent)
{
    qRegisterMetaType<ItemModel*>();

    QHash<int, QByteArray> roles;
    roles[Qt::DisplayRole] = "display";
    setRoleNames(roles);
}

ItemModel::~ItemModel()
{
}

QObject* ItemModel::at(int index) const
{
    return m_items.value(index);
}

int ItemModel::rowCount(const QModelIndex& parent) const
{
    return !parent.isValid() ? m_items.count() : 0;
}

QVariant ItemModel::data(const QModelIndex& index, int role) const
{
    if (role == Qt::DisplayRole)
        return QVariant::fromValue(m_items.value(index.row()));

    return QVariant();
}

void ItemModel::sort(int column, Qt::SortOrder order)
{
    Q_UNUSED(column);
    if (!m_sortProperty.isEmpty())
    {
        emit layoutAboutToBeChanged();

        PropertySorter sorter(m_sortProperty, order);
        qSort(m_items.begin(), m_items.end(), sorter);

        emit layoutChanged();
    }
}

bool ItemModel::contains(const QString& guid) const
{
    return m_guids.contains(guid);
}

QByteArray ItemModel::sortProperty() const
{
    return m_sortProperty;
}

void ItemModel::setSortProperty(const QByteArray& property)
{
    m_sortProperty = property;
}

void ItemModel::clear()
{
    qDeleteAll(m_items);
    m_items.clear();
    m_guids.clear();
    reset();
}
