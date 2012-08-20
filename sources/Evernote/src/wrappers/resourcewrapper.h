#ifndef RESOURCEWRAPPER_H
#define RESOURCEWRAPPER_H
#include "edam/UserStore.h"
#include "edam/UserStore_constants.h"
#include "edam/NoteStore.h"
#include "edam/NoteStore_constants.h"
#include "edam/NoteStore_types.h"
#include "fileutils.h"
#include <QObject>

using namespace evernote::edam;

class ResourceWrapper : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString hash READ getHash)
    Q_PROPERTY(QString guid READ getGuid)
    Q_PROPERTY(QString filename READ getFilename)
    Q_PROPERTY(QString fullPath READ getFullPath)

public:
    explicit ResourceWrapper(QObject *parent = 0);
    ResourceWrapper(Resource r,QObject *parent = 0);
private:
    Resource r;

signals:

public slots:
    QString getHash();
    QString getGuid();
    QString getFilename();
    QString getFullPath();
    static QString convertToHex(std::string plain);
};

#endif // RESOURCEWRAPPER_H
