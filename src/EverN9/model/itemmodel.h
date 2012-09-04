#ifndef ITEMMODEL_H
#define ITEMMODEL_H

#include <QHash>
#include <QMetaType>
#include <QAbstractListModel>

class ItemModel : public QAbstractListModel
{
    Q_OBJECT

public:
    explicit ItemModel(QObject* parent = 0);
    virtual ~ItemModel();

    int rowCount(const QModelIndex& parent = QModelIndex()) const;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;

    Q_INVOKABLE QObjectList items() const;
    Q_INVOKABLE QObject* get(const QString& guid) const;
    Q_INVOKABLE void add(const QObjectList& items);

public slots:
    void clear();

private:
    QList<QObject*> m_items;
    QHash<QString, QObject*> m_guids;
};

Q_DECLARE_METATYPE(ItemModel*)

#endif // ITEMMODEL_H
