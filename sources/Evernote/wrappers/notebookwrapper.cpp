#include "notebookwrapper.h"

NotebookWrapper::NotebookWrapper(QObject *parent) :
    QObject(parent)
{

}
NotebookWrapper::NotebookWrapper(Notebook notebook, QObject *parent):QObject(parent){
    this->notebook = notebook;
}
QString NotebookWrapper::getName(){
    return QString::fromStdString(notebook.name);
}
QString NotebookWrapper::getGuid(){
    return QString::fromStdString(notebook.guid);
}
