#ifndef SESSION_H
#define SESSION_H

#include <QObject>
#include "edam/Types_types.h"

class UserStore;
class Synchronizer;
class ResourceItem;
class NoteItem;

class Session : public QObject
{
    Q_OBJECT

public:
    explicit Session(QObject* parent = 0);
    virtual ~Session();

    UserStore* userStore() const;
    Synchronizer* synchronizer() const;

private slots:
    void onNotebooksSynced(const QVector<evernote::edam::Notebook>& notebooks);
    void onResourcesSynced(const QVector<evernote::edam::Resource>& resources);
    void onNotesSynced(const QVector<evernote::edam::Note>& notes);
    void onTagsSynced(const QVector<evernote::edam::Tag>& tags);
    void onResourceFetched(const evernote::edam::Resource& resource);
    void onNoteFetched(const evernote::edam::Note& note);

private:
    UserStore* m_user;
    Synchronizer* m_sync;
};

#endif // SESSION_H
