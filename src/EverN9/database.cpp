//#define QT_NO_DEBUG_OUTPUT

#include "database.h"
#include "notebookitem.h"
#include "resourceitem.h"
#include "noteitem.h"
#include "tagitem.h"
#include "edam/Types_types.h"
#include <QtConcurrentRun>
#include <QSqlDatabase>
#include <QStringList>
#include <QSqlRecord>
#include <QSqlQuery>
#include <QSqlError>
#include <QFileInfo>
#include <QVariant>
#include <QDebug>

Database::Database(QObject* parent) : QObject(parent)
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("EverN9");
    if (db.open()) {
        QStringList queries;
        queries += "CREATE TABLE IF NOT EXISTS Notebooks(guid TEXT PRIMARY KEY, name TEXT, isDefault INTEGER, isPublished INTEGER, created INTEGER, updated INTEGER)";
        queries += "CREATE TABLE IF NOT EXISTS Resources(guid TEXT PRIMARY KEY, filePath TEXT)";
        queries += "CREATE TABLE IF NOT EXISTS Tags(guid TEXT PRIMARY KEY, name TEXT, parentGuid TEXT)";
        queries += "CREATE TABLE IF NOT EXISTS Notes(guid TEXT PRIMARY KEY, title TEXT, content TEXT, created INTEGER, updated INTEGER, deleted INTEGER, isActive INTEGER, notebookGuid TEXT, tagGuids TEXT, resourceGuids TEXT)";
        foreach (const QString& query, queries)
            db.exec(query);
    }
    bool res = db.isOpen() && !db.lastError().isValid();
    qDebug() << Q_FUNC_INFO << res;
}

Database::~Database()
{
    qDebug() << Q_FUNC_INFO;
    QSqlDatabase::database().close();
    QSqlDatabase::removeDatabase("EverN9");
}

void Database::reset()
{
    QtConcurrent::run(this, &Database::resetImpl);
}

void Database::load(QObject* parent)
{
    QtConcurrent::run(this, &Database::loadImpl, parent);
}

void Database::saveNotebooks(const QList<NotebookItem*>& notebooks)
{
    QtConcurrent::run(this, &Database::saveNotebooksImpl, notebooks);
}

void Database::saveResources(const QList<ResourceItem*>& resources)
{
    QtConcurrent::run(this, &Database::saveResourcesImpl, resources);
}

void Database::saveNotes(const QList<NoteItem*>& notes)
{
    QtConcurrent::run(this, &Database::saveNotesImpl, notes);
}

void Database::saveTags(const QList<TagItem*>& tags)
{
    QtConcurrent::run(this, &Database::saveTagsImpl, tags);
}

void Database::resetImpl()
{
    QStringList queries;
    queries += "DELETE FROM Notebooks";
    queries += "DELETE FROM Resources";
    queries += "DELETE FROM Tags";
    queries += "DELETE FROM Notes";

    QSqlDatabase db = QSqlDatabase::database();
    foreach (const QString& query, queries)
        db.exec(query);
    qDebug() << Q_FUNC_INFO << !db.lastError().isValid();
}

void Database::loadImpl(QObject* parent)
{
    loadNotesImpl(parent);
    loadTagsImpl(parent);
    loadResourcesImpl(parent);
    loadNotebooksImpl(parent);
}

void Database::loadNotebooksImpl(QObject* parent)
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

            NotebookItem* item = new NotebookItem(notebook);
            item->moveToThread(thread());
            item->setParent(parent);
            res += item;
        }
    }
    qDebug() << Q_FUNC_INFO << res.count();
    emit notebooksLoaded(res);
}

void Database::saveNotebooksImpl(const QList<NotebookItem*>& notebooks)
{
    QVariantList guids, names, defs, pubs, creates, updates;
    foreach (NotebookItem* notebook, notebooks) {
        guids += notebook->guid();
        names += notebook->name();
        defs += notebook->isDefault();
        pubs += notebook->isPublished();
        creates += notebook->created().toMSecsSinceEpoch();
        updates += notebook->updated().toMSecsSinceEpoch();
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
}

void Database::loadResourcesImpl(QObject* parent)
{
    QList<ResourceItem*> res;
    QSqlQuery query("SELECT * FROM Resources");
    if (query.exec()) {
        while (query.next()) {
            evernote::edam::Resource resource;
            QSqlRecord record = query.record();
            resource.guid = record.value("guid").toString().toStdString();
            resource.mime = QFileInfo(record.value("filePath").toString()).suffix().toStdString();

            ResourceItem* item = new ResourceItem(resource);
            item->moveToThread(thread());
            item->setParent(parent);
            res += item;
        }
    }
    qDebug() << Q_FUNC_INFO << res.count();
    emit resourcesLoaded(res);
}

void Database::saveResourcesImpl(const QList<ResourceItem*>& resources)
{
    QVariantList guids, filePaths;
    foreach (ResourceItem* resource, resources) {
        guids += resource->guid();
        filePaths += resource->filePath();
    }

    QSqlQuery query("INSERT OR REPLACE INTO Resources VALUES(?,?)");
    query.addBindValue(guids);
    query.addBindValue(filePaths);
    bool res = query.execBatch();
    qDebug() << Q_FUNC_INFO << resources.count() << res;
}

void Database::loadNotesImpl(QObject* parent)
{
    QList<NoteItem*> res;
    QSqlQuery query("SELECT * FROM Notes");
    if (query.exec()) {
        while (query.next()) {
            evernote::edam::Note note;
            QSqlRecord record = query.record();
            note.guid = record.value("guid").toString().toStdString();
            note.title = record.value("title").toString().toStdString();
            note.content = record.value("content").toString().toStdString();
            note.created = record.value("created").toLongLong();
            note.updated = record.value("updated").toLongLong();
            note.deleted = record.value("deleted").toLongLong();
            note.active = record.value("isActive").toBool();
            note.notebookGuid = record.value("notebookGuid").toString().toStdString();
            QStringList tags = record.value("tagGuids").toString().split(";", QString::SkipEmptyParts);
            foreach (const QString& tag, tags)
                note.tagGuids.push_back(tag.toStdString());
            QStringList resources = record.value("resourceGuids").toString().split(";", QString::SkipEmptyParts);
            foreach (const QString& resource, resources) {
                evernote::edam::Resource r;
                r.guid = resource.toStdString();
                note.resources.push_back(r);
            }

            NoteItem* item = new NoteItem(note);
            item->moveToThread(thread());
            item->setParent(parent);
            res += item;
        }
    }
    qDebug() << Q_FUNC_INFO << res.count();
    emit notesLoaded(res);
}

void Database::saveNotesImpl(const QList<NoteItem*>& notes)
{
    QVariantList guids, titles, contents, creates, updates,
                 deletes, actives, notebooks, tags, resources;
    foreach (NoteItem* note, notes) {
        guids += note->guid();
        titles += note->title();
        contents += note->content();
        creates += note->created().toMSecsSinceEpoch();
        updates += note->updated().toMSecsSinceEpoch();
        deletes += note->deleted().toMSecsSinceEpoch();
        actives += note->isActive();
        notebooks += QString::fromStdString(note->note().notebookGuid);
        QStringList tagGuids;
        for (uint i = 0; i < note->note().tagGuids.size(); ++i)
            tagGuids += QString::fromStdString(note->note().tagGuids.at(i));
        tags += tagGuids.join(";");
        QStringList resourceGuids;
        for (uint i = 0; i < note->note().resources.size(); ++i)
            resourceGuids += QString::fromStdString(note->note().resources.at(i).guid);
        resources += resourceGuids.join(";");
    }

    QSqlQuery query("INSERT OR REPLACE INTO Notes VALUES(?,?,?,?,?,?,?,?,?,?)");
    query.addBindValue(guids);
    query.addBindValue(titles);
    query.addBindValue(contents);
    query.addBindValue(creates);
    query.addBindValue(updates);
    query.addBindValue(deletes);
    query.addBindValue(actives);
    query.addBindValue(notebooks);
    query.addBindValue(tags);
    query.addBindValue(resources);
    bool res = query.execBatch();
    qDebug() << Q_FUNC_INFO << notes.count() << res;
}

void Database::loadTagsImpl(QObject* parent)
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

            TagItem* item = new TagItem(tag);
            item->moveToThread(thread());
            item->setParent(parent);
            res += item;
        }
    }
    qDebug() << Q_FUNC_INFO << res.count();
    emit tagsLoaded(res);
}

void Database::saveTagsImpl(const QList<TagItem*>& tags)
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
}
