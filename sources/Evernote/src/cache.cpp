#include "cache.h"
#include <QFileDialog>
#include <QDebug>

#include <QtCore>
Cache* Cache::m_instance = NULL;

Cache::Cache(QObject *parent) :
    QObject(parent)
{
    tags = new QVector <Tag> ();
    notebooks = new QVector <Notebook> ();
    notes = new QVector <Note>();
}
Cache* Cache::instance(){
   if(!m_instance){
        m_instance = new Cache();
   }
   return m_instance;
}
Cache::~Cache() {

}

void Cache::clear(){
    tags->clear();
    notebooks->clear();
    notes->clear();

}
void Cache::load(){
    tags = DatabaseManager::instance()->getTags();
    notebooks = DatabaseManager::instance()->getNotebooks();
    notes = DatabaseManager::instance()->getNotes();
    clearNotes();
    qDebug() << "Notes size: " << notes->size();
    for(int i=0;i<notes->size();i++){
        Note note = notes->at(i);
        NoteWrapper* noteWrapper = new NoteWrapper(note);
        noteAdded(noteWrapper);
    }
}
NoteWrapper* Cache::getNote(int index){
    return new NoteWrapper(notes->at(index));
}
QString Cache::getNoteContent(NoteWrapper* note){
    return FileUtils::readNoteContent(note);
}

void Cache::clearFileCache(){
    FileUtils::removeNoteDir();
}
void Cache::openTestFileWindow(){
    QFileDialog * fd = new QFileDialog(NULL);
    fd->setAcceptMode(QFileDialog::AcceptOpen);
    fd->setFileMode(QFileDialog::ExistingFiles);
    fd->exec();
}
void Cache::fillWithTags(){
    for(int i=0;i<tags->size();i++){
        Tag tag = tags->at(i);
        TagWrapper* wrapper = new TagWrapper(tag);
        tagFired(wrapper);
    }
}
void Cache::fillWithNotebooks(){
    for(int i=0;i<notebooks->size();i++){
        Notebook notebook = notebooks->at(i);
        NotebookWrapper* wrapper = new NotebookWrapper(notebook);
        notebookFired(wrapper);
    }
}
QString Cache::getCacheFileName(NoteWrapper* note){
    return FileUtils::noteContentFilePath(note);
}
NotebookWrapper* Cache::getNotebook(NoteWrapper* note){
    for(int i=0;i<notebooks->size();i++){
        Notebook notebook = notebooks->at(i);
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
    tags = DatabaseManager::instance()->getTags();
    for(int i=0;i<tags->size();i++){
        Tag tag = tags->at(i);
        TagWrapper* tagWrapper = new TagWrapper(tag);
        tagAdded(tagWrapper);
    }
    setTagsLoaded(true);
}*/

