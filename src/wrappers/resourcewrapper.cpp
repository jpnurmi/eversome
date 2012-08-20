#include "resourcewrapper.h"
#include <QDebug>
#include <iostream>
#include <string>
#include <bitset>

ResourceWrapper::ResourceWrapper(QObject *parent) :
    QObject(parent)
{
}

ResourceWrapper::ResourceWrapper(Resource r, QObject *parent){
    this->r = r;
}
QString ResourceWrapper::getGuid(){
    return QString::fromStdString(r.guid);
}

QString ResourceWrapper::getHash(){
    return QString::fromStdString(r.data.bodyHash);
}
QString ResourceWrapper::getFilename(){
    return QString::fromStdString(r.attributes.fileName);
}

QString ResourceWrapper::convertToHex(std::string plain){
    QString result;
    for(int i=0;i< plain.length();i++){
        int c = (int)plain.at(i);
        QString one = QString("%1").arg(c, 2, 16, QChar('0'));
        result += one;
    }
    return result;
}
QString ResourceWrapper::getFullPath(){
    return FileUtils::resourceContentFilePath(r);
}

