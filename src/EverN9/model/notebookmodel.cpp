#include "notebookmodel.h"
#include "notebookitem.h"
#include <QDebug>

NotebookModel::NotebookModel(QObject *parent) : QAbstractListModel(parent)
{
    qRegisterMetaType<NotebookModel*>();

    QHash<int, QByteArray> roles;
    roles[Qt::DisplayRole] = "display";
    setRoleNames(roles);
}

NotebookModel::~NotebookModel()
{
}

NotebookModel* NotebookModel::instance()
{
    static NotebookModel model;
    return &model;
}

int NotebookModel::rowCount(const QModelIndex& parent) const
{
    return !parent.isValid() ? m_notebooks.count() : 0;
}

QVariant NotebookModel::data(const QModelIndex& index, int role) const
{
    if (role == Qt::DisplayRole)
        return QVariant::fromValue(m_notebooks.value(index.row()));

    return QVariant();
}

QList<NotebookItem*> NotebookModel::notebooks() const
{
    return m_notebooks;
}

NotebookItem* NotebookModel::get(const QString& guid) const
{
    return m_guids.value(guid);
}

void NotebookModel::add(const QList<NotebookItem*>& notebooks)
{
    QList<NotebookItem*> unique;
    foreach (NotebookItem* notebook, notebooks) {
        if (m_guids.contains(notebook->guid()))
            continue;
        unique += notebook;
        m_guids.insert(notebook->guid(), notebook);
    }

    if (!unique.isEmpty()) {
        const int row = rowCount();
        const int count = unique.count();
        beginInsertRows(QModelIndex(), row, row + count - 1);
        m_notebooks += unique;
        endInsertRows();
    }
}

void NotebookModel::clear()
{
    qDeleteAll(m_notebooks);
    m_notebooks.clear();
    m_guids.clear();
    reset();
}
