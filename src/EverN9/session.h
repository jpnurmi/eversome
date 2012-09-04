#ifndef SESSION_H
#define SESSION_H

#include <QObject>
#include "edam/Types_types.h"

class UserStore;
class NoteStore;
class NotebookModel;
class ResourceModel;
class TagModel;
class ResourceItem;
class NoteItem;

class Session : public QObject
{
    Q_OBJECT

public:
    explicit Session(QObject* parent = 0);
    virtual ~Session();

    static Session* instance();

    UserStore* userStore() const;
    NoteStore* noteStore() const;

    NotebookModel* notebookModel() const;
    ResourceModel* resourceModel() const;
    TagModel* tagModel() const;

private slots:
    void onLoggedIn();
    void onLoggedOut();
    void onNotebooksSynced(const QVector<evernote::edam::Notebook>& notebooks);
    void onResourcesSynced(const QVector<evernote::edam::Resource>& resources);
    void onNotesSynced(const QVector<evernote::edam::Note>& notes);
    void onTagsSynced(const QVector<evernote::edam::Tag>& tags);
    void onResourceFetched(const evernote::edam::Resource& resource);
    void onNoteFetched(const evernote::edam::Note& note);

private:
    void setupNotes(const QList<NoteItem*>& notes);

    UserStore* m_user;
    NoteStore* m_note;
    NotebookModel* m_notebooks;
    ResourceModel* m_resources;
    TagModel* m_tags;
};

#endif // SESSION_H
