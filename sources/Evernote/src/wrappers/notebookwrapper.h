#ifndef NOTEBOOKWRAPPER_H
#define NOTEBOOKWRAPPER_H
#include "edam/UserStore.h"
#include "edam/UserStore_constants.h"
#include "edam/NoteStore.h"
#include "edam/NoteStore_constants.h"
#include "edam/NoteStore_types.h"


#include <QObject>
using namespace evernote::edam;

class NotebookWrapper : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString name READ getName)
    Q_PROPERTY(QString guid READ getGuid)

public:
    explicit NotebookWrapper(QObject *parent = 0);
    NotebookWrapper(Notebook notebook,QObject *parent = 0);
private:
    Notebook notebook;
signals:

public slots:
    QString getName();
    QString getGuid();
};

#endif // NOTEBOOKWRAPPER_H
