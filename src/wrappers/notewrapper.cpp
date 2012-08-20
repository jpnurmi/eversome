#include "notewrapper.h"
#include <QDate>
#include <QDebug>
#include "fileutils.h"
#include <QVector>
NoteWrapper::NoteWrapper(QObject *parent) :
    QObject(parent)
{
}
NoteWrapper::NoteWrapper(Note note, QObject *parent):QObject(parent){
    this->note  = note;
}
QString NoteWrapper::getTitle(){
    return QString::fromStdString(note.title);

}
Guid NoteWrapper::getGuid(){
    return note.guid;
}
QString NoteWrapper::getDateCreated(){
    QDateTime date;
    date.setTime_t(note.created/1000);
    return date.toString("dd/MM/yyyy");
}
bool NoteWrapper::isCached(){
    return FileUtils::noteCached(this);
 }
QString NoteWrapper::getNoteContentUrl(){
    return FileUtils::noteContentFilePath(this);
}
QString NoteWrapper::getNotebookGUID(){
    return QString::fromStdString(note.notebookGuid);
}
QVariantList NoteWrapper::getTagGuids(){
    QVariantList result;
    qDebug() << note.tagGuids.size();
    for(int i=0;i<note.tagGuids.size();i++){
        result.append(QString::fromStdString(note.tagGuids.at(i)));
    }
    return result;
}
\
