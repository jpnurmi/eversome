#ifndef CACHE_H
#define CACHE_H

#include "edam/Types_types.h"
#include "wrappers/tagwrapper.h"
#include "wrappers/notewrapper.h"
#include "wrappers/notebookwrapper.h"
#include <QObject>
#include <QVector>

class Cache : public QObject
{
    Q_OBJECT

public:
    static Cache* instance();
    Cache(QObject* parent = 0);
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
    void fillWithTags();
    void fillWithNotebooks();
    QString getNoteContent(NoteWrapper* note);
    QString getCacheFileName(NoteWrapper* note);
    NotebookWrapper* getNotebook(NoteWrapper* note);

private:
    QVector<evernote::edam::Notebook> notebooks;
    QVector<evernote::edam::Note> notes;
    QVector<evernote::edam::Tag> tags;
};

#endif // CACHE_H
