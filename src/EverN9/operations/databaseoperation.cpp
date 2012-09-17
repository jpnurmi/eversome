/*
* Copyright (C) 2012 J-P Nurmi <jpnurmi@gmail.com>
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*/
//#define QT_NO_DEBUG_OUTPUT

#include "databaseoperation.h"
#include "operationerror.h"
#include "notebookitem.h"
#include "resourceitem.h"
#include "searchitem.h"
#include "noteitem.h"
#include "tagitem.h"
#include <Types_types.h>
#include <QDesktopServices>
#include <QApplication>
#include <QSqlDatabase>
#include <QStringList>
#include <QSqlRecord>
#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>
#include <QThread>
#include <QDebug>
#include <QDir>

static QString databaseName()
{
    QDir dir(QDesktopServices::storageLocation(QDesktopServices::DataLocation));
    return dir.absoluteFilePath(QApplication::applicationName() + ".db");
}

static QSqlDatabase currentDatabase()
{
    QString id = QString::number(QThread::currentThreadId());
    QSqlDatabase db = QSqlDatabase::database(id);
    if (!db.isValid()) {
        db = QSqlDatabase::addDatabase("QSQLITE", id);
        db.setDatabaseName(databaseName());
        if (!db.open())
            qCritical() << Q_FUNC_INFO << "CANNOT OPEN" << db.lastError().text();
    }
    return db;
}

DatabaseOperation::DatabaseOperation(Mode mode) : Operation(mode)
{
}

DatabaseOperation::~DatabaseOperation()
{
}

bool DatabaseOperation::isValid() const
{
    return true; // TODO
}

void DatabaseOperation::setData(const QList<NotebookItem*>& notebooks,
                                const QList<ResourceItem*>& resources,
                                const QList<SearchItem*>& searches,
                                const QList<NoteItem*>& notes,
                                const QList<TagItem*>& tags)
{
    m_notebooks = notebooks;
    m_resources = resources;
    m_searches = searches;
    m_notes = notes;
    m_tags = tags;
}

void DatabaseOperation::operate()
{
    const char* operation = 0;
    switch (mode())
    {
        case OpenDatabase:
        {
            QStringList queries;
            queries += "CREATE TABLE IF NOT EXISTS Notebooks(guid TEXT PRIMARY KEY, name TEXT, isDefault INTEGER, isPublished INTEGER, created INTEGER, updated INTEGER, usn INTEGER)";
            queries += "CREATE TABLE IF NOT EXISTS Resources(guid TEXT PRIMARY KEY, mime TEXT, hash TEXT, usn INTEGER)";
            queries += "CREATE TABLE IF NOT EXISTS Searches(guid TEXT PRIMARY KEY, name TEXT, query TEXT, usn INTEGER)";
            queries += "CREATE TABLE IF NOT EXISTS Notes(guid TEXT PRIMARY KEY, title TEXT, created INTEGER, updated INTEGER, deleted INTEGER, isActive INTEGER, notebookGuid TEXT, tagGuids TEXT, resourceGuids TEXT, unread INTEGER, usn INTEGER)";
            queries += "CREATE TABLE IF NOT EXISTS Tags(guid TEXT PRIMARY KEY, name TEXT, parentGuid TEXT, usn INTEGER)";

            QSqlDatabase db = currentDatabase();
            foreach (const QString& query, queries)
                db.exec(query);
            break;
        }
        case CloseDatabase:
        {
            foreach (const QString& id, QSqlDatabase::connectionNames()) {
                QSqlDatabase::database(id).close();
                QSqlDatabase::removeDatabase(id);
            }
            break;
        }
        case ResetDatabase:
        {
            QStringList queries;
            queries += "DELETE FROM Notebooks";
            queries += "DELETE FROM Resources";
            queries += "DELETE FROM Searches";
            queries += "DELETE FROM Notes";
            queries += "DELETE FROM Tags";

            QSqlDatabase db = currentDatabase();
            foreach (const QString& query, queries)
                db.exec(query);
            break;
        }
        case LoadDatabase:
        {
            operation = "loaded...";
            QSqlDatabase db = currentDatabase();
            m_notebooks = loadNotebooks(db);
            m_resources = loadResources(db);
            m_searches = loadSearches(db);
            m_notes = loadNotes(db);
            m_tags = loadTags(db);
            emit loaded(m_notebooks, m_resources, m_searches, m_notes, m_tags);
            break;
        }
        case SaveDatabase:
        {
            operation = "saved...";
            QSqlDatabase db = currentDatabase();
            saveNotebooks(m_notebooks, db);
            saveResources(m_resources, db);
            saveSearches(m_searches, db);
            saveNotes(m_notes, db);
            saveTags(m_tags, db);
            break;
        }
        case RemoveDatabase:
        {
            operation = "removed...";
            QSqlDatabase db = currentDatabase();
            removeNotebooks(m_notebooks, db);
            removeSearches(m_searches, db);
            removeNotes(m_notes, db);
            removeTags(m_tags, db);
            break;
        }
        default:
            Q_ASSERT(false);
            break;
    }

    if (operation)
        qDebug() << "DatabaseOperation::operate():" << operation
                 << "NB:" << m_notebooks.count()
                 << "R:" << m_resources.count()
                 << "S:" << m_searches.count()
                 << "N:" << m_notes.count()
                 << "T:" << m_tags.count();
}

QList<NotebookItem*> DatabaseOperation::loadNotebooks(QSqlDatabase db)
{
    QList<NotebookItem*> notebooks;
    QSqlQuery query("SELECT * FROM Notebooks ORDER BY name ASC", db);
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
            notebook.updateSequenceNum = record.value("usn").toInt();

            NotebookItem* item = new NotebookItem(notebook);
            item->moveToThread(thread());
            notebooks += item;
        }
    }
    return notebooks;
}

bool DatabaseOperation::saveNotebooks(const QList<NotebookItem*>& notebooks, QSqlDatabase db)
{
    QVariantList guids, names, defs, pubs, creates, updates, usns;
    foreach (NotebookItem* notebook, notebooks) {
        guids += notebook->guid();
        names += notebook->name();
        defs += notebook->isDefault();
        pubs += notebook->isPublished();
        creates += notebook->created().toMSecsSinceEpoch();
        updates += notebook->updated().toMSecsSinceEpoch();
        usns += notebook->usn();
    }

    QSqlQuery query("INSERT OR REPLACE INTO NoteBooks VALUES(?,?,?,?,?,?,?)", db);
    query.addBindValue(guids);
    query.addBindValue(names);
    query.addBindValue(defs);
    query.addBindValue(pubs);
    query.addBindValue(creates);
    query.addBindValue(updates);
    query.addBindValue(usns);
    return query.execBatch();
}

bool DatabaseOperation::removeNotebooks(const QList<NotebookItem*>& notebooks, QSqlDatabase db)
{
    QVariantList guids;
    foreach (NotebookItem* notebook, notebooks)
        guids += notebook->guid();

    QSqlQuery query("DELETE FROM NoteBooks WHERE guid=?", db);
    query.addBindValue(guids);
    return query.execBatch();
}

QList<ResourceItem*> DatabaseOperation::loadResources(QSqlDatabase db)
{
    QList<ResourceItem*> resources;
    QSqlQuery query("SELECT * FROM Resources", db);
    if (query.exec()) {
        while (query.next()) {
            evernote::edam::Resource resource;
            QSqlRecord record = query.record();
            resource.guid = record.value("guid").toString().toStdString();
            resource.mime = record.value("mime").toString().toStdString();
            resource.data.bodyHash = QByteArray::fromHex(record.value("hash").toByteArray()).constData();
            resource.updateSequenceNum = record.value("usn").toInt();

            ResourceItem* item = new ResourceItem(resource);
            item->moveToThread(thread());
            resources += item;
        }
    }
    return resources;
}

bool DatabaseOperation::saveResources(const QList<ResourceItem*>& resources, QSqlDatabase db)
{
    QVariantList guids, mimes, hashes, usns;
    foreach (ResourceItem* resource, resources) {
        guids += resource->guid();
        mimes += resource->mime();
        hashes += resource->hash();
        usns += resource->usn();
    }

    QSqlQuery query("INSERT OR REPLACE INTO Resources VALUES(?,?,?,?)", db);
    query.addBindValue(guids);
    query.addBindValue(mimes);
    query.addBindValue(hashes);
    query.addBindValue(usns);
    return query.execBatch();
}

bool DatabaseOperation::removeResources(const QList<ResourceItem*>& resources, QSqlDatabase db)
{
    QVariantList guids;
    foreach (ResourceItem* resource, resources)
        guids += resource->guid();

    QSqlQuery query("DELETE FROM Resources WHERE guid=?", db);
    query.addBindValue(guids);
    return query.execBatch();
}

QList<SearchItem*> DatabaseOperation::loadSearches(QSqlDatabase db)
{
    QList<SearchItem*> searches;
    QSqlQuery query("SELECT * FROM Searches ORDER BY name ASC", db);
    if (query.exec()) {
        while (query.next()) {
            evernote::edam::SavedSearch search;
            QSqlRecord record = query.record();
            search.guid = record.value("guid").toString().toStdString();
            search.name = record.value("name").toString().toStdString();
            search.query = record.value("query").toString().toStdString();
            search.updateSequenceNum = record.value("usn").toInt();

            SearchItem* item = new SearchItem(search);
            item->moveToThread(thread());
            searches += item;
        }
    }
    return searches;
}

bool DatabaseOperation::saveSearches(const QList<SearchItem*>& searches, QSqlDatabase db)
{
    QVariantList guids, names, queries, usns;
    foreach (SearchItem* search, searches) {
        guids += search->guid();
        names += search->name();
        queries += search->query();
        usns += search->usn();
    }

    QSqlQuery query("INSERT OR REPLACE INTO Searches VALUES(?,?,?,?)", db);
    query.addBindValue(guids);
    query.addBindValue(names);
    query.addBindValue(queries);
    query.addBindValue(usns);
    return query.execBatch();
}

bool DatabaseOperation::removeSearches(const QList<SearchItem*>& searches, QSqlDatabase db)
{
    QVariantList guids;
    foreach (SearchItem* search, searches)
        guids += search->guid();

    QSqlQuery query("DELETE FROM Searches WHERE guid=?", db);
    query.addBindValue(guids);
    return query.execBatch();
}

QList<NoteItem*> DatabaseOperation::loadNotes(QSqlDatabase db)
{
    QList<NoteItem*> notes;
    QSqlQuery query("SELECT * FROM Notes", db);
    if (query.exec()) {
        while (query.next()) {
            evernote::edam::Note note;
            QSqlRecord record = query.record();
            note.guid = record.value("guid").toString().toStdString();
            note.title = record.value("title").toString().toStdString();
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
            note.updateSequenceNum = record.value("usn").toInt();

            NoteItem* item = new NoteItem(note);
            item->setUnread(record.value("unread").toBool());
            item->moveToThread(thread());
            notes += item;
        }
    }
    return notes;
}

bool DatabaseOperation::saveNotes(const QList<NoteItem*>& notes, QSqlDatabase db)
{
    QVariantList guids, titles, creates, updates, deletes,
                 actives, notebooks, tags, resources, unreads, usns;
    foreach (NoteItem* note, notes) {
        const evernote::edam::Note& data = note->data();
        guids += note->guid();
        titles += note->title();
        creates += note->created().toMSecsSinceEpoch();
        updates += note->updated().toMSecsSinceEpoch();
        deletes += note->deleted().toMSecsSinceEpoch();
        actives += note->isActive();
        notebooks += QString::fromStdString(data.notebookGuid);
        QStringList tagGuids;
        for (uint i = 0; i < data.tagGuids.size(); ++i)
            tagGuids += QString::fromStdString(data.tagGuids.at(i));
        tags += tagGuids.join(";");
        QStringList resourceGuids;
        for (uint i = 0; i < data.resources.size(); ++i)
            resourceGuids += QString::fromStdString(data.resources.at(i).guid);
        resources += resourceGuids.join(";");
        unreads += note->isUnread();
        usns += note->usn();
    }

    QSqlQuery query("INSERT OR REPLACE INTO Notes VALUES(?,?,?,?,?,?,?,?,?,?,?)", db);
    query.addBindValue(guids);
    query.addBindValue(titles);
    query.addBindValue(creates);
    query.addBindValue(updates);
    query.addBindValue(deletes);
    query.addBindValue(actives);
    query.addBindValue(notebooks);
    query.addBindValue(tags);
    query.addBindValue(resources);
    query.addBindValue(unreads);
    query.addBindValue(usns);
    return query.execBatch();
}

bool DatabaseOperation::removeNotes(const QList<NoteItem*>& notes, QSqlDatabase db)
{
    QVariantList guids;
    foreach (NoteItem* note, notes)
        guids += note->guid();

    QSqlQuery query("DELETE FROM Notes WHERE guid=?", db);
    query.addBindValue(guids);
    return query.execBatch();
}

QList<TagItem*> DatabaseOperation::loadTags(QSqlDatabase db)
{
    QList<TagItem*> tags;
    QSqlQuery query("SELECT * FROM Tags ORDER BY name ASC", db);
    if (query.exec()) {
        while (query.next()) {
            evernote::edam::Tag tag;
            QSqlRecord record = query.record();
            tag.guid = record.value("guid").toString().toStdString();
            tag.name = record.value("name").toString().toStdString();
            tag.parentGuid = record.value("parentGuid").toString().toStdString();
            tag.updateSequenceNum = record.value("usn").toInt();

            TagItem* item = new TagItem(tag);
            item->moveToThread(thread());
            tags += item;
        }
    }
    return tags;
}

bool DatabaseOperation::saveTags(const QList<TagItem*>& tags, QSqlDatabase db)
{
    QVariantList guids, names, parents, usns;
    foreach (TagItem* tag, tags) {
        guids += tag->guid();
        names += tag->name();
        parents += tag->parentGuid();
        usns += tag->usn();
    }

    QSqlQuery query("INSERT OR REPLACE INTO Tags VALUES(?,?,?,?)", db);
    query.addBindValue(guids);
    query.addBindValue(names);
    query.addBindValue(parents);
    query.addBindValue(usns);
    return query.execBatch();
}

bool DatabaseOperation::removeTags(const QList<TagItem*>& tags, QSqlDatabase db)
{
    QVariantList guids;
    foreach (TagItem* tag, tags)
        guids += tag->guid();

    QSqlQuery query("DELETE FROM Tags WHERE guid=?", db);
    query.addBindValue(guids);
    return query.execBatch();
}
