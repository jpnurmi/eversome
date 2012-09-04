#ifndef NOTEBOOKITEM_H
#define NOTEBOOKITEM_H

#include <QObject>
#include <QDateTime>
#include <QMetaType>
#include "edam/Types_types.h"
#include "itemmodel.h"

class NotebookItem : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString guid READ guid CONSTANT)
    Q_PROPERTY(QString name READ name CONSTANT)
    Q_PROPERTY(bool isDefault READ isDefault CONSTANT)
    Q_PROPERTY(bool isPublished READ isPublished CONSTANT)
    Q_PROPERTY(QDateTime created READ created CONSTANT)
    Q_PROPERTY(QDateTime updated READ updated CONSTANT)
    Q_PROPERTY(ItemModel* notes READ notes CONSTANT)

public:
    explicit NotebookItem(evernote::edam::Notebook notebook = evernote::edam::Notebook(), QObject* parent = 0);
    virtual ~NotebookItem();

    evernote::edam::Notebook notebook() const;

    QString guid() const;
    QString name() const;
    bool isDefault() const;
    bool isPublished() const;
    QDateTime created() const;
    QDateTime updated() const;
    ItemModel* notes() const;

private:
    ItemModel* m_notes;
    evernote::edam::Notebook m_notebook;
};

Q_DECLARE_METATYPE(NotebookItem*)

#endif // NOTEBOOKITEM_H
