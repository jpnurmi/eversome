#ifndef DATABASE_H
#define DATABASE_H

#include <QList>
#include <QObject>

class TagItem;
class NoteItem;
class ResourceItem;
class NotebookItem;

class Database
{
public:
    static bool initialize();
    static void uninitialize();

    static bool reset();

    static QList<NotebookItem*> loadNotebooks(QObject* parent = 0);
    static bool saveNotebooks(const QList<NotebookItem*>& notebooks);

    static QList<ResourceItem*> loadResources(QObject* parent = 0);
    static bool saveResources(const QList<ResourceItem*>& resources);

    static QList<TagItem*> loadTags(QObject* parent = 0);
    static bool saveTags(const QList<TagItem*>& tags);

    static QList<NoteItem*> loadNotes(QObject* parent = 0);
    static bool saveNotes(const QList<NoteItem*>& notes);
};

#endif // DATABASE_H
