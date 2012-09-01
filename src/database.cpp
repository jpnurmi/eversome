//#define QT_NO_DEBUG_OUTPUT

#include "database.h"
#include "notebookitem.h"
#include "tagitem.h"
#include "edam/Types_types.h"
#include <QSqlDatabase>
#include <QStringList>
#include <QSqlRecord>
#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>
#include <QDebug>

bool Database::initialize()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("EverN9");
    if (db.open()) {
        QStringList queries;
        queries += "CREATE TABLE IF NOT EXISTS Notebooks(guid TEXT PRIMARY KEY, name TEXT, isDefault INTEGER, isPublished INTEGER, created INTEGER, updated INTEGER)";
        queries += "CREATE TABLE IF NOT EXISTS Tags(guid TEXT PRIMARY KEY, name TEXT, parentGuid TEXT)";
        foreach (const QString& query, queries)
            db.exec(query);
    }
    bool res = db.isOpen() && !db.lastError().isValid();
    qDebug() << Q_FUNC_INFO << res;
    return res;
}

void Database::uninitialize()
{
    qDebug() << Q_FUNC_INFO;
    QSqlDatabase::database().close();
    QSqlDatabase::removeDatabase("EverN9");
}

bool Database::reset()
{
    QStringList queries;
    queries += "DELETE FROM Notebooks";
    queries += "DELETE FROM Tags";

    QSqlDatabase db = QSqlDatabase::database();
    foreach (const QString& query, queries)
        db.exec(query);
    qDebug() << Q_FUNC_INFO << !db.lastError().isValid();
    return !db.lastError().isValid();
}

QList<NotebookItem*> Database::loadNotebooks(QObject* parent)
{
    QList<NotebookItem*> res;
    QSqlQuery query("SELECT * FROM Notebooks ORDER BY name ASC");
    if (query.exec()) {
        while (query.next()) {
            evernote::edam::Notebook notebook;
            QSqlRecord record = query.record();
            notebook.guid = record.value("guid").toString().toStdString();
            notebook.name = record.value("name").toString().toStdString();
            notebook.defaultNotebook = record.value("isDefault").toBool();
            notebook.published = record.value("isPublished").toBool();
            notebook.serviceCreated = record.value("created").toLongLong();
            notebook.serviceUpdated = record.value("updated").toLongLong();
            res += new NotebookItem(notebook, parent);
        }
    }
    qDebug() << Q_FUNC_INFO << res.count();
    return res;
}

bool Database::saveNotebooks(const QList<NotebookItem*>& notebooks)
{
    QVariantList guids, names, defs, pubs, creates, updates;
    foreach (NotebookItem* notebook, notebooks) {
        guids += notebook->guid();
        names += notebook->name();
        defs += notebook->isDefault();
        pubs += notebook->isPublished();
        creates += notebook->created();
        updates += notebook->updated();
    }

    QSqlQuery query("INSERT OR REPLACE INTO NoteBooks VALUES(?,?,?,?,?,?)");
    query.addBindValue(guids);
    query.addBindValue(names);
    query.addBindValue(defs);
    query.addBindValue(pubs);
    query.addBindValue(creates);
    query.addBindValue(updates);
    bool res = query.execBatch();
    qDebug() << Q_FUNC_INFO << notebooks.count() << res;
    return res;
}

QList<TagItem*> Database::loadTags(QObject* parent)
{
    QList<TagItem*> res;
    QSqlQuery query("SELECT * FROM Tags ORDER BY name ASC");
    if (query.exec()) {
        while (query.next()) {
            evernote::edam::Tag tag;
            QSqlRecord record = query.record();
            tag.guid = record.value("guid").toString().toStdString();
            tag.name = record.value("name").toString().toStdString();
            tag.parentGuid = record.value("parentGuid").toString().toStdString();
            res += new TagItem(tag, parent);
        }
    }
    qDebug() << Q_FUNC_INFO << res.count();
    return res;
}

bool Database::saveTags(const QList<TagItem*>& tags)
{
    QVariantList guids, names, parents;
    foreach (TagItem* tag, tags) {
        guids += tag->guid();
        names += tag->name();
        parents += tag->parentGuid();
    }

    QSqlQuery query("INSERT OR REPLACE INTO Tags VALUES(?,?,?)");
    query.addBindValue(guids);
    query.addBindValue(names);
    query.addBindValue(parents);
    bool res = query.execBatch();
    qDebug() << Q_FUNC_INFO << tags.count() << res;
    return res;
}
