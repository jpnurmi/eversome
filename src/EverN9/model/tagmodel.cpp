#include "tagmodel.h"
#include "tagitem.h"
#include <QDebug>

TagModel::TagModel(QObject *parent) : QAbstractListModel(parent)
{
    qRegisterMetaType<TagModel*>();

    QHash<int, QByteArray> roles;
    roles[Qt::DisplayRole] = "display";
    setRoleNames(roles);
}

TagModel::~TagModel()
{
}

TagModel* TagModel::instance()
{
    static TagModel model;
    return &model;
}

int TagModel::rowCount(const QModelIndex& parent) const
{
    return !parent.isValid() ? m_tags.count() : 0;
}

QVariant TagModel::data(const QModelIndex& index, int role) const
{
    if (role == Qt::DisplayRole)
        return QVariant::fromValue(m_tags.value(index.row()));

    return QVariant();
}

QList<TagItem*> TagModel::tags() const
{
    return m_tags;
}

TagItem* TagModel::get(const QString& guid) const
{
    return m_guids.value(guid);
}

void TagModel::add(const QList<TagItem*>& tags)
{
    QList<TagItem*> unique;
    foreach (TagItem* tag, tags) {
        if (m_guids.contains(tag->guid()))
            continue;
        unique += tag;
        m_guids.insert(tag->guid(), tag);
    }

    if (!unique.isEmpty()) {
        const int row = rowCount();
        const int count = unique.count();
        beginInsertRows(QModelIndex(), row, row + count - 1);
        m_tags += unique;
        endInsertRows();
    }
}

void TagModel::clear()
{
    qDeleteAll(m_tags);
    m_tags.clear();
    m_guids.clear();
    reset();
}
