#ifndef TAGWRAPPER_H
#define TAGWRAPPER_H
#include "edam/UserStore.h"
#include "edam/UserStore_constants.h"
#include "edam/NoteStore.h"
#include "edam/NoteStore_constants.h"
#include "edam/NoteStore_types.h"
#include <QObject>

using namespace evernote::edam;

class TagWrapper : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString name READ getName)
    Q_PROPERTY(QString guid READ getGuid)

public:
    explicit TagWrapper(QObject *parent = 0);
    TagWrapper(Tag tag,QObject *parent = 0);
private:
    Tag tag;
signals:

public slots:
    QString getName();
    QString getGuid();
};

#endif // TAGWRAPPER_H
