#ifndef NOTEMODEL_H
#define NOTEMODEL_H

#include <QHash>
#include <QMetaType>
#include <QAbstractListModel>

class NoteItem;

class NoteModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(int count READ rowCount NOTIFY countChanged)

public:
    explicit NoteModel(QObject* parent = 0);
    virtual ~NoteModel();

    int rowCount(const QModelIndex& parent = QModelIndex()) const;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;

public slots:
    void add(NoteItem* note);

signals:
    void countChanged();

private:
    QList<NoteItem*> m_notes;

    friend class Session;
    friend class NoteItem;
    static QHash<QString, NoteItem*> allNotes;
};

Q_DECLARE_METATYPE(NoteModel*)

#endif // NOTEMODEL_H
