#ifndef NOTEWRAPPER_H
#define NOTEWRAPPER_H
#include "edam/UserStore.h"
#include "edam/UserStore_constants.h"
#include "edam/NoteStore.h"
#include "edam/NoteStore_constants.h"
#include "edam/NoteStore_types.h"
#include <QVariant>
#include <QObject>
using namespace evernote::edam;
using namespace std;
class NoteWrapper : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString title READ getTitle)
    Q_PROPERTY(QString dateCreated READ getDateCreated)
    Q_PROPERTY(bool cached READ isCached)
    Q_PROPERTY(QString noteContentUrl READ getNoteContentUrl)
    Q_PROPERTY(QString notebookGUID READ getNotebookGUID)
    Q_PROPERTY(QVariantList  tagGuids READ getTagGuids)
public:
    explicit NoteWrapper(QObject *parent = 0);
    NoteWrapper(Note note,QObject *parent = 0);
    Note note;
signals:

public slots:
    QString getTitle();
    Guid getGuid();
    QString getDateCreated();
    bool isCached();
    QString getNoteContentUrl();
    QString getNotebookGUID();
    QVariantList getTagGuids();

private:

};

#endif // NOTEWRAPPER_H
