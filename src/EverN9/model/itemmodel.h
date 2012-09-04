#ifndef ITEMMODEL_H
#define ITEMMODEL_H

#include <QHash>
#include <QMetaType>
#include <QAbstractListModel>

class ItemModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(int count READ rowCount NOTIFY countChanged)

public:
    explicit ItemModel(QObject* parent = 0);
    virtual ~ItemModel();

    int rowCount(const QModelIndex& parent = QModelIndex()) const;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;

    template <typename T>
    QList<T> items() const;

    template <typename T>
    T get(const QString& guid) const;

    template <typename T>
    void add(const QList<T>& items);

public slots:
    void clear();

signals:
    void countChanged();

private:
    QList<QObject*> m_items;
    QHash<QString, QObject*> m_guids;
};

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
void ItemModel::add(const QList<T>& items)
{
    QObjectList unique;
    foreach (T item, items) {
        if (m_guids.contains(item->guid()))
            continue;
        unique += item;
        m_guids.insert(item->guid(), item);
    }

    if (!unique.isEmpty()) {
        const int row = rowCount();
        const int count = unique.count();
        beginInsertRows(QModelIndex(), row, row + count - 1);
        m_items += unique;
        endInsertRows();
        emit countChanged();
    }
}

Q_DECLARE_METATYPE(ItemModel*)

#endif // ITEMMODEL_H