#ifndef NOTEBOOKMODEL_H
#define NOTEBOOKMODEL_H

#include <QHash>
#include <QMetaType>
#include <QAbstractListModel>

class NotebookItem;

class NotebookModel : public QAbstractListModel
{
    Q_OBJECT

public:
    explicit NotebookModel(QObject* parent = 0);
    virtual ~NotebookModel();

    int rowCount(const QModelIndex& parent = QModelIndex()) const;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;

    Q_INVOKABLE QList<NotebookItem*> notebooks() const;
    Q_INVOKABLE NotebookItem* get(const QString& guid) const;
    Q_INVOKABLE void add(const QList<NotebookItem*>& notebooks);

public slots:
    void clear();

private:
    QList<NotebookItem*> m_notebooks;
    QHash<QString, NotebookItem*> m_guids;
};

Q_DECLARE_METATYPE(NotebookModel*)

#endif // NOTEBOOKMODEL_H
