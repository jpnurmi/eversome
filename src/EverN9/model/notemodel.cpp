#include "notemodel.h"
#include "noteitem.h"
#include <QDebug>

NoteModel::NoteModel(QObject *parent) : QAbstractListModel(parent)
{
    qRegisterMetaType<NoteModel*>();

    QHash<int, QByteArray> roles;
    roles[Qt::DisplayRole] = "display";
    setRoleNames(roles);
}

NoteModel::~NoteModel()
{
}

int NoteModel::rowCount(const QModelIndex& parent) const
{
    return !parent.isValid() ? m_notes.count() : 0;
}

QVariant NoteModel::data(const QModelIndex& index, int role) const
{
    if (role == Qt::DisplayRole)
        return QVariant::fromValue(m_notes.value(index.row()));

    return QVariant();
}

void NoteModel::add(NoteItem* note)
{
    const int row = rowCount();
    beginInsertRows(QModelIndex(), row, row);
    m_notes += note;
    endInsertRows();
    emit countChanged();
}
