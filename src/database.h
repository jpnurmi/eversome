#ifndef DATABASE_H
#define DATABASE_H

#include <QList>
#include <QObject>

class TagItem;
class NotebookItem;

class Database
{
public:
    static bool initialize();
    static void uninitialize();

    static bool reset();

    static QList<NotebookItem*> loadNotebooks(QObject* parent = 0);
    static bool saveNotebooks(const QList<NotebookItem*>& notebooks);

    static QList<TagItem*> loadTags(QObject* parent = 0);
    static bool saveTags(const QList<TagItem*>& tags);
};

#endif // DATABASE_H
