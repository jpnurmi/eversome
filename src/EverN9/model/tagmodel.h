#ifndef TAGMODEL_H
#define TAGMODEL_H

#include <QHash>
#include <QMetaType>
#include <QAbstractListModel>

class TagItem;

class TagModel : public QAbstractListModel
{
    Q_OBJECT

public:
    explicit TagModel(QObject* parent = 0);
    virtual ~TagModel();

    static TagModel* instance();

    int rowCount(const QModelIndex& parent = QModelIndex()) const;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;

    Q_INVOKABLE QList<TagItem*> tags() const;
    Q_INVOKABLE TagItem* get(const QString& guid) const;
    Q_INVOKABLE void add(const QList<TagItem*>& tags);

public slots:
    void clear();

private:
    QList<TagItem*> m_tags;
    QHash<QString, TagItem*> m_guids;
};

Q_DECLARE_METATYPE(TagModel*)

#endif // TAGMODEL_H
