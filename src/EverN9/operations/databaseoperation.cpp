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
#include <QDebug>
#include <QDir>

static QString databaseName()
{
    QDir dir(QDesktopServices::storageLocation(QDesktopServices::DataLocation));
    return dir.absoluteFilePath(QApplication::applicationName() + ".db");
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
    switch (mode())
    {
        case OpenDatabase:
        {
            QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
            db.setDatabaseName(databaseName());
            if (db.open()) {
                QStringList queries;
                queries += "CREATE TABLE IF NOT EXISTS Notebooks(guid TEXT PRIMARY KEY, name TEXT, isDefault INTEGER, isPublished INTEGER, created INTEGER, updated INTEGER, usn INTEGER)";
                queries += "CREATE TABLE IF NOT EXISTS Resources(guid TEXT PRIMARY KEY, mime TEXT, usn INTEGER)";
                queries += "CREATE TABLE IF NOT EXISTS Searches(guid TEXT PRIMARY KEY, name TEXT, query TEXT, usn INTEGER)";
                queries += "CREATE TABLE IF NOT EXISTS Notes(guid TEXT PRIMARY KEY, title TEXT, content TEXT, created INTEGER, updated INTEGER, deleted INTEGER, isActive INTEGER, notebookGuid TEXT, tagGuids TEXT, resourceGuids TEXT, usn INTEGER)";
                queries += "CREATE TABLE IF NOT EXISTS Tags(guid TEXT PRIMARY KEY, name TEXT, parentGuid TEXT, usn INTEGER)";
                foreach (const QString& query, queries)
                    db.exec(query);
            }
            break;
        }
        case CloseDatabase:
        {
            QSqlDatabase::database().close();
            QSqlDatabase::removeDatabase(databaseName());
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

            QSqlDatabase db = QSqlDatabase::database();
            foreach (const QString& query, queries)
                db.exec(query);
            break;
        }
        case LoadDatabase:
        {
            emit loaded(loadNotebooks(), loadResources(), loadSearches(), loadNotes(), loadTags());
            break;
        }
        case SaveDatabase:
        {
            saveNotebooks(m_notebooks);
            saveResources(m_resources);
            saveSearches(m_searches);
            saveNotes(m_notes);
            saveTags(m_tags);
            break;
        }
        case RemoveDatabase:
        {
            removeNotebooks(m_notebooks);
            removeSearches(m_searches);
            removeNotes(m_notes);
            removeTags(m_tags);
        }
        default:
            qWarning() << Q_FUNC_INFO << "unknown mode" << mode();
            break;
    }

    if (mode() != CloseDatabase)
        qDebug() << Q_FUNC_INFO << this << !QSqlDatabase::database().lastError().isValid();
}

QList<NotebookItem*> DatabaseOperation::loadNotebooks()
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
            notebook.updateSequenceNum = record.value("usn").toInt();

            NotebookItem* item = new NotebookItem(notebook);
            item->moveToThread(thread());
            res += item;
        }
    }
    qDebug() << Q_FUNC_INFO << res.count();
    return res;
}

void DatabaseOperation::saveNotebooks(const QList<NotebookItem*>& notebooks)
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

    QSqlQuery query("INSERT OR REPLACE INTO NoteBooks VALUES(?,?,?,?,?,?,?)");
    query.addBindValue(guids);
    query.addBindValue(names);
    query.addBindValue(defs);
    query.addBindValue(pubs);
    query.addBindValue(creates);
    query.addBindValue(updates);
    query.addBindValue(usns);
    bool res = query.execBatch();
    qDebug() << Q_FUNC_INFO << notebooks.count() << res;
}

void DatabaseOperation::removeNotebooks(const QList<NotebookItem*>& notebooks)
{
    QVariantList guids;
    foreach (NotebookItem* notebook, notebooks)
        guids += notebook->guid();

    QSqlQuery query("DELETE FROM NoteBooks WHERE guid=?");
    query.addBindValue(guids);
    bool res = query.execBatch();
    qDebug() << Q_FUNC_INFO << notebooks.count() << res;
}

QList<ResourceItem*> DatabaseOperation::loadResources()
{
    QList<ResourceItem*> res;
    QSqlQuery query("SELECT * FROM Resources");
    if (query.exec()) {
        while (query.next()) {
            evernote::edam::Resource resource;
            QSqlRecord record = query.record();
            resource.guid = record.value("guid").toString().toStdString();
            resource.mime = record.value("mime").toString().toStdString();
            resource.updateSequenceNum = record.value("usn").toInt();

            ResourceItem* item = new ResourceItem(resource);
            item->moveToThread(thread());
            res += item;
        }
    }
    qDebug() << Q_FUNC_INFO << res.count();
    return res;
}

void DatabaseOperation::saveResources(const QList<ResourceItem*>& resources)
{
    QVariantList guids, mimes, usns;
    foreach (ResourceItem* resource, resources) {
        guids += resource->guid();
        mimes += resource->mime();
        usns += resource->usn();
    }

    QSqlQuery query("INSERT OR REPLACE INTO Resources VALUES(?,?,?)");
    query.addBindValue(guids);
    query.addBindValue(mimes);
    query.addBindValue(usns);
    bool res = query.execBatch();
    qDebug() << Q_FUNC_INFO << resources.count() << res;
}

void DatabaseOperation::removeResources(const QList<ResourceItem*>& resources)
{
    QVariantList guids;
    foreach (ResourceItem* resource, resources)
        guids += resource->guid();

    QSqlQuery query("DELETE FROM Resources WHERE guid=?");
    query.addBindValue(guids);
    bool res = query.execBatch();
    qDebug() << Q_FUNC_INFO << resources.count() << res;
}

QList<SearchItem*> DatabaseOperation::loadSearches()
{
    QList<SearchItem*> res;
    QSqlQuery query("SELECT * FROM Searches ORDER BY name ASC");
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
            res += item;
        }
    }
    qDebug() << Q_FUNC_INFO << res.count();
    return res;
}

void DatabaseOperation::saveSearches(const QList<SearchItem*>& searches)
{
    QVariantList guids, names, queries, usns;
    foreach (SearchItem* search, searches) {
        guids += search->guid();
        names += search->name();
        queries += search->query();
        usns += search->usn();
    }

    QSqlQuery query("INSERT OR REPLACE INTO Searches VALUES(?,?,?,?)");
    query.addBindValue(guids);
    query.addBindValue(names);
    query.addBindValue(queries);
    query.addBindValue(usns);
    bool res = query.execBatch();
    qDebug() << Q_FUNC_INFO << searches.count() << res;
}

void DatabaseOperation::removeSearches(const QList<SearchItem*>& searches)
{
    QVariantList guids;
    foreach (SearchItem* search, searches)
        guids += search->guid();

    QSqlQuery query("DELETE FROM Searches WHERE guid=?");
    query.addBindValue(guids);
    bool res = query.execBatch();
    qDebug() << Q_FUNC_INFO << searches.count() << res;
}

QList<NoteItem*> DatabaseOperation::loadNotes()
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
            note.updateSequenceNum = record.value("usn").toInt();

            NoteItem* item = new NoteItem(note);
            item->moveToThread(thread());
            res += item;
        }
    }
    qDebug() << Q_FUNC_INFO << res.count();
    return res;
}

void DatabaseOperation::saveNotes(const QList<NoteItem*>& notes)
{
    QVariantList guids, titles, contents, creates, updates,
                 deletes, actives, notebooks, tags, resources, usns;
    foreach (NoteItem* note, notes) {
        const evernote::edam::Note& data = note->data();
        guids += note->guid();
        titles += note->title();
        contents += note->content();
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
        usns += note->usn();
    }

    QSqlQuery query("INSERT OR REPLACE INTO Notes VALUES(?,?,?,?,?,?,?,?,?,?,?)");
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
    query.addBindValue(usns);
    bool res = query.execBatch();
    qDebug() << Q_FUNC_INFO << notes.count() << res;
}

void DatabaseOperation::removeNotes(const QList<NoteItem*>& notes)
{
    QVariantList guids;
    foreach (NoteItem* note, notes)
        guids += note->guid();

    QSqlQuery query("DELETE FROM Notes WHERE guid=?");
    query.addBindValue(guids);
    bool res = query.execBatch();
    qDebug() << Q_FUNC_INFO << notes.count() << res;
}

QList<TagItem*> DatabaseOperation::loadTags()
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
            tag.updateSequenceNum = record.value("usn").toInt();

            TagItem* item = new TagItem(tag);
            item->moveToThread(thread());
            res += item;
        }
    }
    qDebug() << Q_FUNC_INFO << res.count();
    return res;
}

void DatabaseOperation::saveTags(const QList<TagItem*>& tags)
{
    QVariantList guids, names, parents, usns;
    foreach (TagItem* tag, tags) {
        guids += tag->guid();
        names += tag->name();
        parents += tag->parentGuid();
        usns += tag->usn();
    }

    QSqlQuery query("INSERT OR REPLACE INTO Tags VALUES(?,?,?,?)");
    query.addBindValue(guids);
    query.addBindValue(names);
    query.addBindValue(parents);
    query.addBindValue(usns);
    bool res = query.execBatch();
    qDebug() << Q_FUNC_INFO << tags.count() << res;
}

void DatabaseOperation::removeTags(const QList<TagItem*>& tags)
{
    QVariantList guids;
    foreach (TagItem* tag, tags)
        guids += tag->guid();

    QSqlQuery query("DELETE FROM Tags WHERE guid=?");
    query.addBindValue(guids);
    bool res = query.execBatch();
    qDebug() << Q_FUNC_INFO << tags.count() << res;
}
