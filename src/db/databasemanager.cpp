#include "databasemanager.h"

const QString DatabaseManager::DB_NAME = "EvernoteDb";

DatabaseManager::DatabaseManager(QObject *parent) :
    QObject(parent)
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(DB_NAME);
    bool ok = db.open();
    if (ok)
        dbOpened();
    else
        dbOpenError();
    createTables();
}

DatabaseManager* DatabaseManager::instance()
{
    static DatabaseManager manager;
    return &manager;
}

DatabaseManager::~DatabaseManager()
{
}

void DatabaseManager::close()
{
    QSqlDatabase db = QSqlDatabase::database();
    db.removeDatabase(DatabaseManager::DB_NAME);
    db.close();
}

void DatabaseManager::createTables()
{
    QStringList queries;
    queries.append(DatabaseConstants::CREATE_SETTINGS_TABLE_QUERY);
    queries.append(DatabaseConstants::CREATE_TAGS_TABLE_QUERY);
    queries.append(DatabaseConstants::CREATE_NOTEBOOKS_TABLE_QUERY);
    queries.append(DatabaseConstants::CREATE_NOTES_TABLE_QUERY);
    queries.append(DatabaseConstants::CREATE_NOTE_TAGS_TABLE_QUERY);
    queries.append(DatabaseConstants::CREATE_RESOURCES_TABLE);

    QSqlDatabase db = QSqlDatabase::database();
    foreach (const QString& query, queries)
        db.exec(query);
}

void DatabaseManager::makeSetting(const QString &key, const QString &value)
{
    QSqlQuery query(DatabaseConstants::CREATE_UPDATE_SETTING_QUERY);
    query.addBindValue(key);
    query.addBindValue(value);
    bool ok = query.exec();
    if(ok){
        qDebug() << "DatabaseManager: setting made (" << key << ")";
    }else{
        qDebug() << "DatabaseManager: cannot create settings ("<<key<<", "<<value<<") : "<<query.lastError();
    }
}

void DatabaseManager::makeIntSetting(const QString &key, int value)
{
    makeSetting(key, QString::number(value));
}

QString DatabaseManager::getSetting(const QString &key)
{
    QSqlQuery query(DatabaseConstants::GET_SETTING_QUERY);
    query.addBindValue(key);
    bool ok = query.exec();

    if(ok){
        qDebug() << "DatabaseManager: setting got (" << key << ")";
    }else{
        qDebug() << "DatabaseManager: cannot get settings ("<<key <<" " <<query.lastError();
    }
    query.next();
    return query.record().value("value").toString();
}

int DatabaseManager::getIntSetting(const QString& key)
{
    QString value = getSetting(key);
    return value.toInt();
}

void DatabaseManager::dropTable(const QString& tableName)
{
    QSqlDatabase db = QSqlDatabase::database();
    db.exec(QString(DatabaseConstants::DROP_TABLE_QUERY).arg(tableName));
}

void DatabaseManager::clear()
{
    dropTable(DatabaseConstants::SETTINGS_TABLE);
    dropTable(DatabaseConstants::NOTEBOOKS_TABLE);
    dropTable(DatabaseConstants::TAGS_TABLE);
    dropTable(DatabaseConstants::NOTES_TABLE);
}

bool DatabaseManager::saveTag(Tag tag)
{
    QSqlQuery query(DatabaseConstants::INSERT_TAG_QUERY);
    query.addBindValue(QString::fromStdString(tag.guid.c_str()));
    query.addBindValue(QString::fromStdString(tag.parentGuid.c_str()));
    query.addBindValue(tag.updateSequenceNum);
    query.addBindValue(QString::fromStdString(tag.name).toUtf8());
    bool ok = query.exec();
    if(ok){
        qDebug() << "DatabaseManager :: tag saved (" << tag.name.c_str() << ")";
    }else{
        qDebug() << "DatabaseManager :: cannot save tag ("<<tag.name.c_str()<< ") : "<<query.lastError();
    }
}

QVector<Tag> DatabaseManager::getTags() const
{
    QVector<Tag> result;
    QSqlQuery query(DatabaseConstants::SELECT_TAGS_QUERY);
    bool ok = query.exec();
    while(query.next()){
        /*qDebug() << "Got tag :" << query.record().value("name").toString() << ","
                 << query.record().value("usn").toInt() << ","
                 << query.record().value("guid").toString() << ","
                    << query.record().value("parentGuid").toString();*/
        Tag tag;
        tag.name = query.record().value("name").toString().toStdString();
        tag.updateSequenceNum = query.record().value("usn").toInt();
        tag.guid = query.record().value("guid").toString().toStdString();
        tag.parentGuid = query.record().value("parentGuid").toString().toStdString();
        result.append(tag);
    }
    return result;
}

bool DatabaseManager::saveNotebook(Notebook notebook)
{
    QSqlQuery query(DatabaseConstants::INSERT_NOTEBOOK_QUERY);
    query.addBindValue(QString::fromStdString(notebook.guid));
    query.addBindValue(QString::fromStdString(notebook.name));
    query.addBindValue(notebook.updateSequenceNum);
    bool ok = query.exec();
    if(ok){
        qDebug() << "DatabaseManager :: notebook saved (" << notebook.name.c_str() << ")";
    }else{
        qDebug() << "DatabaseManager :: cannot save notebook ("<<notebook.name.c_str()<< ") : "<<query.lastError();
    }
}

QVector<Notebook> DatabaseManager::getNotebooks() const
{
    QVector<Notebook> result;
    QSqlQuery query(DatabaseConstants::SELECT_NOTEBOOKS_QUERY);
    bool ok = query.exec();
    while(query.next()){
       /* qDebug() << "Got notebook :" << query.record().value("name").toString() << ","
                 << query.record().value("usn").toInt() << ","
                 << query.record().value("guid").toString();*/
        Notebook notebook;
        notebook.name = query.record().value("name").toString().toStdString();
        notebook.updateSequenceNum = query.record().value("usn").toInt();
        notebook.guid = query.record().value("guid").toString().toStdString();
        result.append(notebook);
    }
    return result;
}

void DatabaseManager::deleteNote(Note note)
{
    QSqlQuery query(DatabaseConstants::DELETE_NOTE);
    query.addBindValue(QString::fromStdString(note.guid));
    bool ok = query.exec();
    if(ok){
        qDebug() << "DatabaseManager :: note deleted (" << note.title.c_str() << ")";
    }else{
        qDebug() << "DatabaseManager :: cannot delete note ("<<note.title.c_str()<< ") : "<<query.lastError();
    }
    deleteNoteTags(note);
    deleteNoteResources(note);
    FileUtils::removeNoteCache(note);
}

bool DatabaseManager::saveNote(Note note)
{
    deleteNote(note);

    QSqlQuery query(DatabaseConstants::INSERT_NOTE_QUERY);

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
    bool ok = query.exec();
    if(ok){
        qDebug() << "DatabaseManager :: note saved (" << note.title.c_str() << ")";
    }else{
        qDebug() << "DatabaseManager :: cannot save note ("<<note.title.c_str()<< ") : "<<query.lastError();
    }

    saveNoteTags(note);
    saveResources(note);
}

bool DatabaseManager::saveNoteTags(Note note)
{
    for(int i=0;i<note.tagGuids.size();i++){
        QSqlQuery noteTagQuery(DatabaseConstants::INSERT_NOTE_TAG_QUERY);
        noteTagQuery.addBindValue(QString::fromStdString(note.guid));
        noteTagQuery.addBindValue(QString::fromStdString((std::string)note.tagGuids.at(i)));
        if(noteTagQuery.exec()){
            qDebug() << "created reference for tag: " << ((std::string)note.tagGuids.at(i)).c_str();
        }else{
            qDebug() << "error: " << noteTagQuery.lastError();
        }
    }
}

void DatabaseManager::deleteNoteTags(Note note)
{
    QSqlQuery noteTagQuery(DatabaseConstants::DELETE_NOTE_TAGS);
    noteTagQuery.addBindValue(QString::fromStdString(note.guid));
    noteTagQuery.exec();
}

bool DatabaseManager::saveResources(Note note)
{
/*
        "guid TEXT PRIMARY KEY,"+
        "noteGuid TEXT,"+
        "dataHash TEXT,"+
        "dataLength TEXT,"+
        "mime TEXT,"+
        "attachment INTEGER,"+
        "filename TEXT"
        +")";

*/
    for(int i=0;i<note.resources.size();i++){
        QSqlQuery noteTagQuery(DatabaseConstants::INSERT_RESOURCE);
        Resource r = note.resources.at(i);

        noteTagQuery.addBindValue(QString::fromStdString(r.guid));
        noteTagQuery.addBindValue(QString::fromStdString(note.guid));
        noteTagQuery.addBindValue(ResourceWrapper::convertToHex(r.data.bodyHash));
        noteTagQuery.addBindValue(r.data.size);
        noteTagQuery.addBindValue(QString::fromStdString(r.mime));
        noteTagQuery.addBindValue(r.attributes.attachment);
        noteTagQuery.addBindValue(QString::fromStdString(r.attributes.fileName));

        if(noteTagQuery.exec()){
            qDebug() << "created reference for resource: " << r.guid.c_str();
        }else{
            qDebug() << "create resource error: " << noteTagQuery.lastError();
        }
    }
}

void DatabaseManager::deleteNoteResources(Note note)
{
    QSqlQuery noteTagQuery(DatabaseConstants::DELETE_NOTE_RESOURCES);
    noteTagQuery.addBindValue(QString::fromStdString(note.guid));
    noteTagQuery.exec();
}

std::vector <std::string> DatabaseManager::getNoteTagGuids(Note note)
{
    std::vector <std::string> tagGuids;
    QSqlQuery query(DatabaseConstants::SELECT_TAGS_FOR_NOTE);
    query.addBindValue(QString::fromStdString(note.guid));

    if(!query.exec()){
        qDebug() << "get tags error: " << query.lastError();

    }
    while(query.next()){
        std::string guid = query.record().value("tagGuid").toString().toStdString();
        tagGuids.push_back(guid);
    }
    return tagGuids;
}

std::vector <Resource> DatabaseManager::getNoteResources(Note note)
{
    std::vector <Resource> result;
    QSqlQuery query(DatabaseConstants::SELECT_RESOURCES_FOR_NOTE);
    /*
            "guid TEXT PRIMARY KEY,"+
            "noteGuid TEXT,"+
            "dataHash TEXT,"+
            "dataLength NUMBER,"+
            "mime TEXT,"+
            "attachment INTEGER,"+
            "filename TEXT"
            +")";

    */

    query.addBindValue(QString::fromStdString(note.guid));
    if(!query.exec()){
        qDebug() << "select resources failed " << query.lastError();
    }
    while(query.next()){
        Resource r;
        r.guid = query.record().value("guid").toString().toStdString();
        //qDebug() << "got resource: "<< r.guid.c_str();
        r.noteGuid = query.record().value("noteGuid").toString().toStdString();
        r.data.bodyHash = query.record().value("dataHash").toString().toStdString();
       // qDebug() << "hash after insert: " << QString::fromStdString(r.data.bodyHash);

        r.data.size = query.record().value("dataLength").toInt();
        r.mime = query.record().value("mime").toString().toStdString();
        r.attributes.attachment = query.record().value("attachment").toBool();
        r.attributes.fileName = query.record().value("filename").toString().toStdString();
        result.push_back(r);
    }
    return result;
}

QVector<Note> DatabaseManager::getNotes() const
{
    QVector<Note> result;
    QSqlQuery query(DatabaseConstants::SELECT_ALL_NOTES_QUERY);
    bool ok = query.exec();
    while(query.next()){
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
        /*query.addBindValue(note.attributes.subjectDate);
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
        */

        result.append(note);
    }
    return result;
}

void DatabaseManager::beginTransacton()
{
    QSqlDatabase db = QSqlDatabase::database();
    db.transaction();
}

void DatabaseManager::commitTransaction()
{
    QSqlDatabase db = QSqlDatabase::database();
    db.commit();
}
