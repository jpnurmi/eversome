#include "resourcemodel.h"
#include "resourceitem.h"
#include <QDebug>

ResourceModel::ResourceModel(QObject *parent) : QAbstractListModel(parent)
{
    qRegisterMetaType<ResourceModel*>();

    QHash<int, QByteArray> roles;
    roles[Qt::DisplayRole] = "display";
    setRoleNames(roles);
}

ResourceModel::~ResourceModel()
{
}

int ResourceModel::rowCount(const QModelIndex& parent) const
{
    return !parent.isValid() ? m_resources.count() : 0;
}

QVariant ResourceModel::data(const QModelIndex& index, int role) const
{
    if (role == Qt::DisplayRole)
        return QVariant::fromValue(m_resources.value(index.row()));

    return QVariant();
}

QList<ResourceItem*> ResourceModel::resources() const
{
    return m_resources;
}

ResourceItem* ResourceModel::get(const QString& guid) const
{
    return m_guids.value(guid);
}

void ResourceModel::add(const QList<ResourceItem*>& resources)
{
    QList<ResourceItem*> unique;
    foreach (ResourceItem* resource, resources) {
        if (m_guids.contains(resource->guid()))
            continue;
        unique += resource;
        m_guids.insert(resource->guid(), resource);
    }

    if (!unique.isEmpty()) {
        const int row = rowCount();
        const int count = unique.count();
        beginInsertRows(QModelIndex(), row, row + count - 1);
        m_resources += unique;
        endInsertRows();
    }
}

void ResourceModel::clear()
{
    qDeleteAll(m_resources);
    m_resources.clear();
    m_guids.clear();
    reset();
}
