#ifndef DATABASE_H
#define DATABASE_H

#include <QList>
#include <QObject>

class TagItem;
class NoteItem;
class ResourceItem;
class NotebookItem;

class Database : public QObject
{
    Q_OBJECT

public:
    explicit Database(QObject* parent = 0);
    virtual ~Database();

    void reset();

    void load(QObject* parent = 0);

    void saveNotebooks(const QList<NotebookItem*>& notebooks);
    void saveResources(const QList<ResourceItem*>& resources);
    void saveNotes(const QList<NoteItem*>& notes);
    void saveTags(const QList<TagItem*>& tags);

signals:
    void notebooksLoaded(const QList<NotebookItem*>& notebooks);
    void resourcesLoaded(const QList<ResourceItem*>& resources);
    void notesLoaded(const QList<NoteItem*>& notes);
    void tagsLoaded(const QList<TagItem*>& tags);

private slots:
    void resetImpl();
    void loadImpl(QObject* parent);

    void loadNotebooksImpl(QObject* parent);
    void saveNotebooksImpl(const QList<NotebookItem*>& notebooks);

    void loadResourcesImpl(QObject* parent);
    void saveResourcesImpl(const QList<ResourceItem*>& resources);

    void loadNotesImpl(QObject* parent);
    void saveNotesImpl(const QList<NoteItem*>& notes);

    void loadTagsImpl(QObject* parent);
    void saveTagsImpl(const QList<TagItem*>& tags);
};

#endif // DATABASE_H
