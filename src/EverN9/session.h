#ifndef SESSION_H
#define SESSION_H

#include <QObject>
#include "edam/Types_types.h"

class Database;
class UserStore;
class NoteStore;
class ItemModel;
class NotebookItem;
class ResourceItem;
class NoteItem;
class TagItem;

class Session : public QObject
{
    Q_OBJECT

public:
    explicit Session(QObject* parent = 0);
    virtual ~Session();

    static Session* instance();

    UserStore* userStore() const;
    NoteStore* noteStore() const;

    ItemModel* notebookModel() const;
    ItemModel* resourceModel() const;
    ItemModel* noteModel() const;
    ItemModel* tagModel() const;

private slots:
    void onLoggedIn();
    void onLoggedOut();

    void onNotebooksLoaded(const QList<NotebookItem*>& notebooks);
    void onResourcesLoaded(const QList<ResourceItem*>& resources);
    void onNotesLoaded(const QList<NoteItem*>& notes);
    void onTagsLoaded(const QList<TagItem*>& tags);

    void onNotebooksSynced(const QVector<evernote::edam::Notebook>& notebooks);
    void onResourcesSynced(const QVector<evernote::edam::Resource>& resources);
    void onNotesSynced(const QVector<evernote::edam::Note>& notes);
    void onTagsSynced(const QVector<evernote::edam::Tag>& tags);

    void onResourceFetched(const evernote::edam::Resource& resource);
    void onNoteFetched(const evernote::edam::Note& note);

private:
    void setupNotebooks(const QList<NoteItem*>& notes);
    void setupTags(const QList<NoteItem*>& notes);

    Database* m_database;
    UserStore* m_user;
    NoteStore* m_note;
    ItemModel* m_notebooks;
    ItemModel* m_resources;
    ItemModel* m_notes;
    ItemModel* m_tags;
};

#endif // SESSION_H
