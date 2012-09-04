#ifndef DATABASE_H
#define DATABASE_H

#include <QList>
#include <QObject>

class TagItem;
class NoteItem;
class SearchItem;
class ResourceItem;
class NotebookItem;

class Database : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool active READ isActive NOTIFY activeChanged)

public:
    explicit Database(QObject* parent = 0);
    virtual ~Database();

    bool isActive() const;

    void reset();

    void load(QObject* parent = 0);
    void save(const QList<NotebookItem*>& notebooks,
              const QList<ResourceItem*>& resources,
              const QList<SearchItem*>& searches,
              const QList<NoteItem*>& notes,
              const QList<TagItem*>& tags);

signals:
    void activeChanged();
    void loaded(const QList<NotebookItem*>& notebooks,
                const QList<ResourceItem*>& resources,
                const QList<SearchItem*>& searches,
                const QList<NoteItem*>& notes,
                const QList<TagItem*>& tags);

private slots:
    void resetImpl();
    void loadImpl(QObject* parent);
    void saveImpl(const QList<NotebookItem*>& notebooks,
                  const QList<ResourceItem*>& resources,
                  const QList<SearchItem*>& searches,
                  const QList<NoteItem*>& notes,
                  const QList<TagItem*>& tags);

    QList<NotebookItem*> loadNotebooksImpl(QObject* parent);
    void saveNotebooksImpl(const QList<NotebookItem*>& notebooks);

    QList<ResourceItem*> loadResourcesImpl(QObject* parent);
    void saveResourcesImpl(const QList<ResourceItem*>& resources);

    QList<SearchItem*> loadSearchesImpl(QObject* parent);
    void saveSearchesImpl(const QList<SearchItem*>& searches);

    QList<NoteItem*> loadNotesImpl(QObject* parent);
    void saveNotesImpl(const QList<NoteItem*>& notes);

    QList<TagItem*> loadTagsImpl(QObject* parent);
    void saveTagsImpl(const QList<TagItem*>& tags);

private:
    volatile bool loading;
    volatile bool saving;
};

#endif // DATABASE_H
