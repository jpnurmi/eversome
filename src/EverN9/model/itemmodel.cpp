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

void ItemModel::clear()
{
    qDeleteAll(m_items);
    m_items.clear();
    m_guids.clear();
    reset();
}
