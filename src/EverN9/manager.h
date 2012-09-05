#ifndef MANAGER_H
#define MANAGER_H

#include <QObject>
#include "edam/Types_types.h"

class Database;
class ResourceWriter;
class UserStore;
class NoteStore;
class ItemModel;
class NotebookItem;
class ResourceItem;
class SearchItem;
class NoteItem;
class TagItem;

class Manager : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool isBusy READ isBusy NOTIFY isBusyChanged)

public:
    explicit Manager(QObject* parent = 0);
    virtual ~Manager();

    bool isBusy() const;

    Database* database() const;
    ResourceWriter* resourceWriter() const;

    UserStore* userStore() const;
    NoteStore* noteStore() const;

    ItemModel* notebookModel() const;
    ItemModel* resourceModel() const;
    ItemModel* searchModel() const;
    ItemModel* noteModel() const;
    ItemModel* tagModel() const;

    static QString errorString(int code);

signals:
    void isBusyChanged();

private slots:
    void onLoggedIn();
    void onLoggedOut();

    void onLoaded(const QList<NotebookItem*>& notebooks,
                  const QList<ResourceItem*>& resources,
                  const QList<SearchItem*>& searches,
                  const QList<NoteItem*>& notes,
                  const QList<TagItem*>& tags);

    void onSynced(const QVector<evernote::edam::Notebook>& notebooks,
                  const QVector<evernote::edam::Resource>& resources,
                  const QVector<evernote::edam::SavedSearch>& searches,
                  const QVector<evernote::edam::Note>& notes,
                  const QVector<evernote::edam::Tag>& tags);

    void onResourceFetched(const evernote::edam::Resource& resource);
    void onNoteFetched(const evernote::edam::Note& note);

    void onResourceWritten(const QString& filePath);

    void onSearched(const evernote::edam::SavedSearch& search, const QVector<evernote::edam::Note>& notes);

private:
    void setupNotes(const QList<NoteItem*>& notes);

    Database* m_database;
    ResourceWriter* m_writer;
    UserStore* m_user;
    NoteStore* m_note;
    ItemModel* m_notebooks;
    ItemModel* m_resources;
    ItemModel* m_searches;
    ItemModel* m_notes;
    ItemModel* m_tags;
};

#endif // MANAGER_H
