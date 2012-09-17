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
#ifndef ITEMMODEL_P_H
#define ITEMMODEL_P_H

#include <QList>
#include <QString>

template <typename T>
QList<T> ItemModel::items() const
{
    QList<T> items;
    foreach (QObject* item, m_items) {
        T t = qobject_cast<T>(item);
        if (t)
            items += t;
    }
    return items;
}

template <typename T>
T ItemModel::get(const QString& guid) const
{
    return qobject_cast<T>(m_guids.value(guid));
}

template <typename T>
bool ItemModel::add(T item)
{
    if (!m_guids.contains(item->guid())) {
        const int row = rowCount();
        beginInsertRows(QModelIndex(), row, row);
        m_guids.insert(item->guid(), item);
        m_items += item;
        endInsertRows();
        emit countChanged();
        emit added(item);
        return true;
    } else {
        replace(item);
    }
    return false;
}

template <typename T>
bool ItemModel::add(const QList<T>& items)
{
    QObjectList unique;
    foreach (T item, items) {
        QString guid = item->guid();
        if (m_guids.contains(guid)) {
            replace(item);
        } else {
            unique += item;
            m_guids.insert(guid, item);
        }
    }

    if (!unique.isEmpty()) {
        const int row = rowCount();
        const int count = unique.count();
        beginInsertRows(QModelIndex(), row, row + count - 1);
        m_items += unique;
        endInsertRows();
        emit countChanged();
        emit added(unique);
        return true;
    }
    return false;
}

template <typename T>
bool ItemModel::replace(T item)
{
    QString guid = item->guid();
    if (m_guids.contains(guid)) {
        T old = qobject_cast<T>(m_guids.value(guid));
        int idx = m_items.indexOf(old);
        m_items.replace(idx, item);
        emit dataChanged(index(idx), index(idx));
        emit removed(old);
        emit added(item);
        old->deleteLater();
        return true;
    }
    return false;
}

template <typename T>
bool ItemModel::remove(T item)
{
    QString guid = item->guid();
    if (m_guids.contains(guid)) {
        int idx = m_items.indexOf(item);
        beginRemoveRows(QModelIndex(), idx, idx);
        m_guids.remove(guid);
        m_items.removeAt(idx);
        endRemoveRows();
        emit countChanged();
        emit removed(item);
        return true;
    }
    return false;
}

#endif // ITEMMODEL_P_H
