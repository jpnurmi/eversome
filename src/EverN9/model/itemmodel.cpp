#include "itemmodel.h"
#include <QDebug>

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

QObjectList ItemModel::items() const
{
    return m_items;
}

QObject* ItemModel::get(const QString& guid) const
{
    return m_guids.value(guid);
}

void ItemModel::add(const QObjectList& items)
{
    QObjectList unique;
    foreach (QObject* item, items) {
        QString guid = item->property("guid").toString();
        if (m_guids.contains(guid))
            continue;
        unique += item;
        m_guids.insert(guid, item);
    }

    if (!unique.isEmpty()) {
        const int row = rowCount();
        const int count = unique.count();
        beginInsertRows(QModelIndex(), row, row + count - 1);
        m_items += unique;
        endInsertRows();
    }
}

void ItemModel::clear()
{
    qDeleteAll(m_items);
    m_items.clear();
    m_guids.clear();
    reset();
}
