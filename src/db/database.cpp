#define QT_NO_DEBUG_OUTPUT

#include "database.h"
#include "wrappers/resourcewrapper.h"
#include <QSqlDatabase>
#include <QStringList>
#include <QSqlRecord>
#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>
#include <QDebug>

using namespace evernote::edam;

bool Database::initialize()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("EverN9");
    if (db.open()) {
        QStringList queries;
        queries += "CREATE TABLE IF NOT EXISTS Notebooks (guid TEXT PRIMARY KEY, name TEXT, usn NUMBER)";
        queries += "CREATE TABLE IF NOT EXISTS Notes (guid TEXT PRIMARY KEY, title TEXT, contentHash TEXT, contentLength NUMBER, created NUMBER, updated NUMBER, deleted NUMBER, active INTEGER, usn NUMBER, notebookGUID TEXT, subjectDate NUMBER, latitude REAL, longitude REAL, altitude REAL, author TEXT, source TEXT, sourceURL TEXT, sourceApplication TEXT, shareDate NUMBER, taskData NUMBER, taskCompleteDate NUMBER, taskDueDate NUMBER, placeName TEXT, contentClass TEXT)";
        queries += "CREATE TABLE IF NOT EXISTS Tags (guid TEXT PRIMARY KEY, parentGuid TEXT, usn NUMBER, name TEXT )";
        queries += "CREATE TABLE IF NOT EXISTS NoteTags (noteGuid TEXT, tagGuid TEXT)";
        queries += "CREATE TABLE IF NOT EXISTS Resources (guid TEXT PRIMARY KEY, noteGuid TEXT, dataHash TEXT, dataLength NUMBER, mime TEXT, attachment INTEGER, filename TEXT)";
        foreach (const QString& query, queries)
            db.exec(query);
    }
    return db.isOpen() && !db.lastError().isValid();
}

void Database::uninitialize()
{
    QSqlDatabase::database().close();
    QSqlDatabase::removeDatabase("EverN9");
}

bool Database::reset()
{
    QStringList queries;
    queries += "DELETE FROM Notebooks";
    queries += "DELETE FROM Notes";
    queries += "DELETE FROM Tags";
    queries += "DELETE FROM NoteTags";
    queries += "DELETE FROM Resources";

    QSqlDatabase db = QSqlDatabase::database();
    foreach (const QString& query, queries)
        db.exec(query);
    return !db.lastError().isValid();
}

bool Database::saveTag(const Tag& tag)
{
    QSqlQuery query("INSERT OR REPLACE INTO Tags VALUES(?,?,?,?)");
    query.addBindValue(QString::fromStdString(tag.guid.c_str()));
    query.addBindValue(QString::fromStdString(tag.parentGuid.c_str()));
    query.addBindValue(tag.updateSequenceNum);
    query.addBindValue(QString::fromStdString(tag.name).toUtf8());
    bool res = query.exec();
    qDebug() << "Database::saveTag(" << tag.name.c_str() << "):" << res;
    return res;
}

QVector<Tag> Database::getTags()
{
    QVector<Tag> res;
    QSqlQuery query("SELECT * FROM Tags ORDER BY name ASC");
    if (query.exec()) {
        while (query.next()) {
            Tag tag;
            tag.name = query.record().value("name").toString().toStdString();
            tag.updateSequenceNum = query.record().value("usn").toInt();
            tag.guid = query.record().value("guid").toString().toStdString();
            tag.parentGuid = query.record().value("parentGuid").toString().toStdString();
            res += tag;
        }
    }
    qDebug() << "Database::getTags():" << res.count();
    return res;
}

bool Database::saveNotebook(const Notebook& notebook)
{
    QSqlQuery query("INSERT OR REPLACE INTO NoteBooks VALUES(?,?,?)");
    query.addBindValue(QString::fromStdString(notebook.guid));
    query.addBindValue(QString::fromStdString(notebook.name));
    query.addBindValue(notebook.updateSequenceNum);
    bool res = query.exec();
    qDebug() << "Database::saveNotebook(" << notebook.name.c_str() << "):" << res;
    return res;
}

QVector<Notebook> Database::getNotebooks()
{
    QVector<Notebook> res;
    QSqlQuery query("SELECT * FROM Notebooks ORDER BY name ASC");
    if (query.exec()) {
        while (query.next()) {
            Notebook notebook;
            notebook.name = query.record().value("name").toString().toStdString();
            notebook.updateSequenceNum = query.record().value("usn").toInt();
            notebook.guid = query.record().value("guid").toString().toStdString();
            res += notebook;
        }
    }
    qDebug() << "Database::getNotebooks():" << res.count();
    return res;
}

bool Database::deleteNote(const Note& note)
{
    QSqlQuery query("DELETE FROM Notes WHERE guid = ?");
    query.addBindValue(QString::fromStdString(note.guid));
    bool res = query.exec();
    qDebug() << "Database::deleteNote(" << note.title.c_str() << "):" << res;
    return res && deleteNoteTags(note) && deleteNoteResources(note);
}

bool Database::saveNote(const Note& note)
{
    QSqlQuery query("INSERT OR REPLACE INTO Notes VALUES(?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?)");
    query.addBindValue(QString::fromStdString(note.guid));
    query.addBindValue(QString::fromStdString(note.title));
    query.addBindValue(QString::fromStdString(note.contentHash));
    query.addBindValue(note.contentLength);
    query.addBindValue(note.created);
    query.addBindValue(note.updated);
    query.addBindValue(note.deleted);
    query.addBindValue(note.active);
    query.addBindValue(note.updateSequenceNum);
    query.addBindValue(QString::fromStdString(note.notebookGuid));
    query.addBindValue(note.attributes.subjectDate);
    query.addBindValue(note.attributes.latitude);
    query.addBindValue(note.attributes.longitude);
    query.addBindValue(note.attributes.altitude);
    query.addBindValue(QString::fromStdString(note.attributes.author));
    query.addBindValue(QString::fromStdString(note.attributes.source));
    query.addBindValue(QString::fromStdString(note.attributes.sourceURL));
    query.addBindValue(QString::fromStdString(note.attributes.sourceApplication));
    query.addBindValue(note.attributes.shareDate);
    query.addBindValue(note.attributes.taskDate);
    query.addBindValue(note.attributes.taskCompleteDate);
    query.addBindValue(note.attributes.taskDueDate);
    query.addBindValue(QString::fromStdString(note.attributes.placeName));
    query.addBindValue(QString::fromStdString(note.attributes.contentClass));
    bool res = query.exec();
    qDebug() << "Database::saveNote(" << note.title.c_str() << "):" << res;
    return res && saveNoteTags(note) && saveNoteResources(note);
}

bool Database::saveNoteTags(const Note& note)
{
    QVariantList notes, tags;
    for (uint i = 0; i < note.tagGuids.size(); ++i) {
        notes += QString::fromStdString(note.guid);
        tags += QString::fromStdString(note.tagGuids.at(i));
    }

    QSqlQuery query("INSERT OR REPLACE INTO NoteTags VALUES(?,?)");
    query.addBindValue(notes);
    query.addBindValue(tags);
    bool res = query.execBatch();
    qDebug() << "Database::saveNoteTags(" << note.title.c_str() << "):" << res;
    return res;
}

bool Database::deleteNoteTags(const Note& note)
{
    QSqlQuery query("DELETE FROM NoteTags WHERE noteGuid = ?");
    query.addBindValue(QString::fromStdString(note.guid));
    bool res = query.exec();
    qDebug() << "Database::deleteNoteTags(" << note.title.c_str() << "):" << res;
    return res;
}

bool Database::saveNoteResources(const Note& note)
{
    QVariantList resources, notes, hashes, lengths, mimes, attachments, filenames;
    for (uint i = 0; i < note.resources.size(); ++i) {
        Resource resource = note.resources.at(i);
        resources += QString::fromStdString(resource.guid);
        notes += QString::fromStdString(note.guid);
        hashes += ResourceWrapper::convertToHex(resource.data.bodyHash);
        lengths += resource.data.size;
        mimes += QString::fromStdString(resource.mime);
        attachments += resource.attributes.attachment;
        filenames += QString::fromStdString(resource.attributes.fileName);
    }

    QSqlQuery query("INSERT OR REPLACE INTO Resources VALUES (?,?,?,?,?,?,?)");
    query.addBindValue(resources);
    query.addBindValue(notes);
    query.addBindValue(hashes);
    query.addBindValue(lengths);
    query.addBindValue(mimes);
    query.addBindValue(attachments);
    query.addBindValue(filenames);
    bool res = query.execBatch();
    qDebug() << "Database::saveNoteResources(" << note.title.c_str() << "):" << res;
    return res;
}

bool Database::deleteNoteResources(const Note& note)
{
    QSqlQuery query("DELETE FROM Resources WHERE noteGuid = ?");
    query.addBindValue(QString::fromStdString(note.guid));
    bool res = query.exec();
    qDebug() << "Database::deleteNoteResources(" << note.title.c_str() << "):" << res;
    return res;
}

QVector<std::string> Database::getNoteTagGuids(const Note& note)
{
    QVector<std::string> res;
    QSqlQuery query("SELECT * FROM NoteTags WHERE noteGuid = ?");
    query.addBindValue(QString::fromStdString(note.guid));
    if (query.exec()) {
        while (query.next())
            res += query.record().value("tagGuid").toString().toStdString();
    }
    return res;
}

QVector<Resource> Database::getNoteResources(const Note& note)
{
    QVector<Resource> res;
    QSqlQuery query("SELECT * FROM Resources WHERE noteGuid = ?");
    query.addBindValue(QString::fromStdString(note.guid));
    if (query.exec()) {
        while (query.next()) {
            Resource resource;
            resource.guid = query.record().value("guid").toString().toStdString();
            resource.noteGuid = query.record().value("noteGuid").toString().toStdString();
            resource.data.bodyHash = query.record().value("dataHash").toString().toStdString();
            resource.data.size = query.record().value("dataLength").toInt();
            resource.mime = query.record().value("mime").toString().toStdString();
            resource.attributes.attachment = query.record().value("attachment").toBool();
            resource.attributes.fileName = query.record().value("filename").toString().toStdString();
            res += resource;
        }
    }
    return res;
}

QVector<Note> Database::getNotes()
{
    QVector<Note> res;
    QSqlQuery query("SELECT * FROM Notes ORDER BY created DESC");
    if (query.exec()) {
        while (query.next()) {
            Note note;
            note.guid = query.record().value("guid").toString().toStdString();
            note.title = query.record().value("title").toString().toStdString();
            note.contentHash = query.record().value("contentHash").toString().toStdString();
            note.contentLength = query.record().value("contentLength").toInt();
            note.created = query.record().value("created").toLongLong();
            note.updated = query.record().value("updated").toInt();
            note.deleted = query.record().value("deleted").toInt();
            note.active = query.record().value("active").toBool();
            note.updateSequenceNum = query.record().value("usn").toInt();
            note.notebookGuid = query.record().value("notebookGUID").toString().toStdString();
            res += note;
        }
    }
    qDebug() << "Database::getNotes():" << res.count();
    return res;
}

void Database::beginTransaction()
{
    QSqlDatabase db = QSqlDatabase::database();
    db.transaction();
}

void Database::commitTransaction()
{
    QSqlDatabase db = QSqlDatabase::database();
    db.commit();
}
