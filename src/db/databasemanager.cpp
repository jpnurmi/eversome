#include "databasemanager.h"

DatabaseManager* DatabaseManager::m_instance = NULL;
const QString DatabaseManager::DB_NAME = "EvernoteDb";

DatabaseManager::DatabaseManager(QObject *parent) :
    QObject(parent)
{
    qDebug() << "DatabaseManager created" << endl;
    db = new QSqlDatabase(QSqlDatabase::addDatabase("QSQLITE"));
    db->setDatabaseName(DB_NAME);
    qDebug()  << "DatabaseManager: opening db..." << endl;
    bool ok = db->open();
    if(ok){
        qDebug()  << "DatabaseManager: db opened." << endl;
        dbOpened();
    }else{
        qDebug()  << "DatabaseManager: db open error" << endl;
        dbOpenError();
    }
    createTables();
}
DatabaseManager* DatabaseManager::instance(){
    if(!m_instance){
        m_instance = new DatabaseManager();
    }
    return m_instance;
}

void DatabaseManager::drop(){
    if(m_instance){
        delete m_instance;
        m_instance = 0;
    }
}
DatabaseManager::~DatabaseManager() {
    if(db){
        delete db;
        db = 0;
    }
}

void DatabaseManager::close(){
    qDebug() << "DatabaseManager closed" << endl;
    if(db){
        db->removeDatabase(DatabaseManager::DB_NAME);
        db->close();
    }
}


void DatabaseManager::createTables(){
    QVector <QString> queries;
    queries.append(DatabaseConstants::CREATE_SETTINGS_TABLE_QUERY);
    queries.append(DatabaseConstants::CREATE_TAGS_TABLE_QUERY);
    queries.append(DatabaseConstants::CREATE_NOTEBOOKS_TABLE_QUERY);
    queries.append(DatabaseConstants::CREATE_NOTES_TABLE_QUERY);
    queries.append(DatabaseConstants::CREATE_NOTE_TAGS_TABLE_QUERY);
    queries.append(DatabaseConstants::CREATE_RESOURCES_TABLE);

    for(int i=0;i<queries.size();i++){
        db->exec(queries.at(i));
    }

}

void DatabaseManager::makeSetting(const QString &key, const QString &value){
    QSqlQuery query = QSqlQuery(DatabaseConstants::CREATE_UPDATE_SETTING_QUERY, *db);
    query.addBindValue(key);
    query.addBindValue(value);
    bool ok = query.exec();
    if(ok){
        qDebug() << "DatabaseManager: setting made (" << key << ")";
    }else{
        qDebug() << "DatabaseManager: cannot create settings ("<<key<<", "<<value<<") : "<<query.lastError();
    }

}
void DatabaseManager::makeIntSetting(const QString &key, const int &value){
    makeSetting(key, QString::number(value));
}

QString DatabaseManager::getSetting(const QString &key){
    QSqlQuery query = QSqlQuery(DatabaseConstants::GET_SETTING_QUERY, *db);
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
int DatabaseManager::getIntSetting(const QString& key){
    QString value = getSetting(key);
    return value.toInt();
}


void DatabaseManager::dropTable(const QString& tableName){
    QString text = QString(DatabaseConstants::DROP_TABLE_QUERY).arg(tableName);
    db->exec(text);
}
void DatabaseManager::clear(){
    dropTable(DatabaseConstants::SETTINGS_TABLE);
    dropTable(DatabaseConstants::NOTEBOOKS_TABLE);
    dropTable(DatabaseConstants::TAGS_TABLE);
    dropTable(DatabaseConstants::NOTES_TABLE);
}
bool DatabaseManager::saveTag(Tag tag){
    QSqlQuery query = QSqlQuery(DatabaseConstants::INSERT_TAG_QUERY, *db);
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
QVector <Tag>* DatabaseManager::getTags(){
    QVector <Tag>* result = new QVector <Tag>();

    QSqlQuery query = QSqlQuery(DatabaseConstants::SELECT_TAGS_QUERY, *db);
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
        result->append(tag);
    }
    return result;
}
bool DatabaseManager::saveNotebook(Notebook notebook){
    QSqlQuery query = QSqlQuery(DatabaseConstants::INSERT_NOTEBOOK_QUERY, *db);
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
QVector <Notebook>* DatabaseManager::getNotebooks(){
    QVector <Notebook>* result = new QVector <Notebook>();

    QSqlQuery query = QSqlQuery(DatabaseConstants::SELECT_NOTEBOOKS_QUERY, *db);
    bool ok = query.exec();
    while(query.next()){
       /* qDebug() << "Got notebook :" << query.record().value("name").toString() << ","
                 << query.record().value("usn").toInt() << ","
                 << query.record().value("guid").toString();*/
        Notebook notebook;
        notebook.name = query.record().value("name").toString().toStdString();
        notebook.updateSequenceNum = query.record().value("usn").toInt();
        notebook.guid = query.record().value("guid").toString().toStdString();
        result->append(notebook);
    }
    return result;
}
void DatabaseManager::deleteNote(Note note){
    QSqlQuery query = QSqlQuery(DatabaseConstants::DELETE_NOTE, *db);
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

bool DatabaseManager::saveNote(Note note){
    deleteNote(note);

    QSqlQuery query = QSqlQuery(DatabaseConstants::INSERT_NOTE_QUERY, *db);

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
bool DatabaseManager::saveNoteTags(Note note){
    for(int i=0;i<note.tagGuids.size();i++){
        QSqlQuery noteTagQuery = QSqlQuery(DatabaseConstants::INSERT_NOTE_TAG_QUERY, *db);
        noteTagQuery.addBindValue(QString::fromStdString(note.guid));
        noteTagQuery.addBindValue(QString::fromStdString((std::string)note.tagGuids.at(i)));
        if(noteTagQuery.exec()){
            qDebug() << "created reference for tag: " << ((std::string)note.tagGuids.at(i)).c_str();
        }else{
            qDebug() << "error: " << noteTagQuery.lastError();
        }
    }
}
void DatabaseManager::deleteNoteTags(Note note){
    QSqlQuery noteTagQuery = QSqlQuery(DatabaseConstants::DELETE_NOTE_TAGS, *db);
    noteTagQuery.addBindValue(QString::fromStdString(note.guid));
    noteTagQuery.exec();
}

bool DatabaseManager::saveResources(Note note){
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
        QSqlQuery noteTagQuery = QSqlQuery(DatabaseConstants::INSERT_RESOURCE, *db);
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
void DatabaseManager::deleteNoteResources(Note note){
    QSqlQuery noteTagQuery = QSqlQuery(DatabaseConstants::DELETE_NOTE_RESOURCES, *db);
    noteTagQuery.addBindValue(QString::fromStdString(note.guid));
    noteTagQuery.exec();
}

std::vector <std::string> DatabaseManager::getNoteTagGuids(Note note){
    std::vector <std::string> tagGuids;
    QSqlQuery query = QSqlQuery(DatabaseConstants::SELECT_TAGS_FOR_NOTE, *db);
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
std::vector <Resource> DatabaseManager::getNoteResources(Note note){
    std::vector <Resource> result;
    QSqlQuery query = QSqlQuery(DatabaseConstants::SELECT_RESOURCES_FOR_NOTE, *db);
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

QVector <Note>* DatabaseManager::getNotes(){
    QVector <Note>* result = new QVector <Note>();

    QSqlQuery query = QSqlQuery(DatabaseConstants::SELECT_ALL_NOTES_QUERY, *db);
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

        result->append(note);
    }
    return result;
}

void DatabaseManager::beginTransacton(){
    this->db->transaction();
}
void DatabaseManager::commitTransaction(){
    this->db->commit();
}
