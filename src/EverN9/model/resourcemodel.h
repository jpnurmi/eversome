#ifndef RESOURCEMODEL_H
#define RESOURCEMODEL_H

#include <QHash>
#include <QMetaType>
#include <QAbstractListModel>

class ResourceItem;

class ResourceModel : public QAbstractListModel
{
    Q_OBJECT

public:
    explicit ResourceModel(QObject* parent = 0);
    virtual ~ResourceModel();

    static ResourceModel* instance();

    int rowCount(const QModelIndex& parent = QModelIndex()) const;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;

    Q_INVOKABLE QList<ResourceItem*> resources() const;
    Q_INVOKABLE ResourceItem* get(const QString& guid) const;
    Q_INVOKABLE void add(const QList<ResourceItem*>& resources);

public slots:
    void clear();

private:
    QList<ResourceItem*> m_resources;
    QHash<QString, ResourceItem*> m_guids;
};

Q_DECLARE_METATYPE(ResourceModel*)

#endif // RESOURCEMODEL_H
