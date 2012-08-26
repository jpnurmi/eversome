#include "cache.h"
#include "database.h"
#include "fileutils.h"

Cache::Cache(QObject *parent) :
    QObject(parent)
{
}

Cache* Cache::instance()
{
    static Cache cache;
    return &cache;
}

Cache::~Cache()
{
}

void Cache::clear()
{
    tags.clear();
    notebooks.clear();
    notes.clear();
}

void Cache::load()
{
    tags = Database::getTags();
    notebooks = Database::getNotebooks();
    notes = Database::getNotes();
    clearNotes();
    for(int i=0;i<notes.size();i++){
        Note note = notes.at(i);
        NoteWrapper* noteWrapper = new NoteWrapper(note);
        noteAdded(noteWrapper);
    }
}

NoteWrapper* Cache::getNote(int index)
{
    return new NoteWrapper(notes.at(index));
}

QString Cache::getNoteContent(NoteWrapper* note)
{
    return FileUtils::readNoteContent(note);
}

void Cache::clearFileCache()
{
    FileUtils::removeNoteDir();
}

void Cache::fillWithTags()
{
    for(int i=0;i<tags.size();i++){
        Tag tag = tags.at(i);
        TagWrapper* wrapper = new TagWrapper(tag);
        tagFired(wrapper);
    }
}

void Cache::fillWithNotebooks()
{
    for(int i=0;i<notebooks.size();i++){
        Notebook notebook = notebooks.at(i);
        NotebookWrapper* wrapper = new NotebookWrapper(notebook);
        notebookFired(wrapper);
    }
}

QString Cache::getCacheFileName(NoteWrapper* note)
{
    return FileUtils::noteContentFilePath(note);
}

NotebookWrapper* Cache::getNotebook(NoteWrapper* note)
{
    for(int i=0;i<notebooks.size();i++){
        Notebook notebook = notebooks.at(i);
        if(notebook.guid.compare(note->getNotebookGUID().toStdString()) == 0){
            return new NotebookWrapper(notebook);
        }
    }
}

/*void Cache::loadTags(){
    if(isTagsLoaded() || EvernoteSession::instance()->isSyncInProgress()){
       return;
    }
    clearTags();
    tags = Database::getTags();
    for(int i=0;i<tags.size();i++){
        Tag tag = tags.at(i);
        TagWrapper* tagWrapper = new TagWrapper(tag);
        tagAdded(tagWrapper);
    }
    setTagsLoaded(true);
}*/

