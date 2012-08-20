#ifndef CACHE_H
#define CACHE_H
#include "edam/UserStore.h"
#include "edam/UserStore_constants.h"
#include "edam/NoteStore.h"
#include "edam/NoteStore_constants.h"
#include "edam/NoteStore_types.h"
#include "wrappers/tagwrapper.h"
#include "wrappers/notewrapper.h"
#include "wrappers/notebookwrapper.h"
#include "db/databaseconstants.h"
#include "db/databasemanager.h"
#include "settings.h"
#include "evernotesession.h"
#include <QtCore>
#include <QObject>
#include <QStringListModel>
using namespace evernote::edam;
using namespace std;

class Cache : public QObject
{
    Q_OBJECT
public:
    static Cache* instance();
    Cache(QObject *parent = 0);
    ~Cache();

signals:
    void clearNotes();
    void noteAdded(NoteWrapper* note);
    void tagFired(TagWrapper* tag);
    void notebookFired(NotebookWrapper* notebook);
public slots:
    void load();
    void clear();
    NoteWrapper* getNote(int index);
    void clearFileCache();
    void openTestFileWindow();
    void fillWithTags();
    void fillWithNotebooks();
    QString getNoteContent(NoteWrapper* note);
    QString getCacheFileName(NoteWrapper* note);
    NotebookWrapper* getNotebook(NoteWrapper* note);
private:
    static Cache* m_instance;
    QVector <Tag>* tags;
    QVector <Notebook>* notebooks;
    QVector <Note>* notes;
};

#endif // CACHE_H
