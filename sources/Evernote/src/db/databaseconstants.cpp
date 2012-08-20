#include "databaseconstants.h"

const QString DatabaseConstants::DROP_TABLE_QUERY = "DROP TABLE IF EXISTS '%1'";

const QString DatabaseConstants::CREATE_TABLE ="CREATE TABLE ";
const QString DatabaseConstants::INSERT_REPLACE = "INSERT OR REPLACE INTO ";
const QString DatabaseConstants::SETTINGS_TABLE = "settings";
const QString DatabaseConstants::CREATE_SETTINGS_TABLE_QUERY = CREATE_TABLE+SETTINGS_TABLE + " (key TEXT PRIMARY KEY, value TEXT)";
const QString DatabaseConstants::GET_SETTING_QUERY = "SELECT value FROM "+SETTINGS_TABLE+" WHERE key=?";
const QString DatabaseConstants::CREATE_UPDATE_SETTING_QUERY = INSERT_REPLACE+SETTINGS_TABLE +" (key,value) VALUES (?,?)";

const QString DatabaseConstants::TAGS_TABLE = "tags";
const QString DatabaseConstants::CREATE_TAGS_TABLE_QUERY = CREATE_TABLE+TAGS_TABLE + " (guid TEXT PRIMARY KEY, parentGuid TEXT, usn NUMBER, name TEXT )";
const QString DatabaseConstants::INSERT_TAG_QUERY= INSERT_REPLACE+TAGS_TABLE+" (guid, parentGuid, usn, name) VALUES(?,?,?,?)";
const QString DatabaseConstants::SELECT_TAGS_QUERY = "SELECT * FROM "+TAGS_TABLE +" ORDER BY name ASC";

const QString DatabaseConstants::NOTEBOOKS_TABLE = "notebooks";
const QString DatabaseConstants::CREATE_NOTEBOOKS_TABLE_QUERY= CREATE_TABLE + NOTEBOOKS_TABLE + " (guid TEXT PRIMARY KEY, name TEXT, usn NUMBER)";
const QString DatabaseConstants::INSERT_NOTEBOOK_QUERY = INSERT_REPLACE+NOTEBOOKS_TABLE+" (guid, name, usn) VALUES(?,?,?)";
const QString DatabaseConstants::SELECT_NOTEBOOKS_QUERY = "SELECT * FROM "+NOTEBOOKS_TABLE +" ORDER BY name ASC";

const QString DatabaseConstants::NOTES_TABLE="notes";
/*
  Guid guid;
  std::string title;
  std::string content;
  std::string contentHash;
  int32_t contentLength;
  Timestamp created;
  Timestamp updated;
  Timestamp deleted;
  bool active;
  int32_t updateSequenceNum;
  std::string notebookGuid;
  std::vector<Guid>  tagGuids;
  std::vector<Resource>  resources;
  NoteAttributes attributes{

  Timestamp subjectDate;
  double latitude;
  double longitude;
  double altitude;
  std::string author;
  std::string source;
  std::string sourceURL;
  std::string sourceApplication;
  Timestamp shareDate;
  Timestamp taskDate;
  Timestamp taskCompleteDate;
  Timestamp taskDueDate;
  std::string placeName;
  std::string contentClass;
  LazyMap applicationData;



}
  std::vector<std::string>  tagNames;

*/

const QString DatabaseConstants::CREATE_NOTES_TABLE_QUERY=CREATE_TABLE+NOTES_TABLE +
        " ("+
        "guid TEXT PRIMARY KEY,"+
        "title TEXT,"+
        "contentHash TEXT,"+
        "contentLength NUMBER,"+
        "created NUMBER,"+
        "updated NUMBER, "+
        "deleted NUMBER, "+
        "active INTEGER, "+
        "usn NUMBER, "+
        "notebookGUID TEXT,"+
        "subjectDate NUMBER, "+
        "latitude REAL, "+
        "longitude REAL, "+
        "altitude REAL, "+
        "author TEXT, "+
        "source TEXT, "+
        "sourceURL TEXT, "+
        "sourceApplication TEXT, "+
        "shareDate NUMBER, "+
        "taskData NUMBER, "+
        "taskCompleteDate NUMBER, "+
        "taskDueDate NUMBER,"+
        "placeName TEXT, "+
        "contentClass TEXT"+
        ")";
const QString DatabaseConstants::INSERT_NOTE_QUERY=INSERT_REPLACE+NOTES_TABLE+" ("+
        "guid,"+
        "title,"+
         "contentHash,"+
       "contentLength,"+
        "created ,"+
        "updated , "+
        "deleted , "+
        "active , "+
        "usn , "+
        "notebookGUID ,"+
        "subjectDate , "+
        "latitude , "+
        "longitude , "+
        "altitude , "+
        "author , "+
        "source , "+
        "sourceURL , "+
        "sourceApplication , "+
        "shareDate , "+
        "taskData , "+
        "taskCompleteDate , "+
        "taskDueDate ,"+
        "placeName , "+
        "contentClass "+
        ") VALUES(?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?)";
const QString DatabaseConstants::SELECT_ALL_NOTES_QUERY="SELECT * FROM "+NOTES_TABLE +" ORDER BY created DESC";
const QString DatabaseConstants::DELETE_NOTE = "DELETE FROM "+NOTES_TABLE +" WHERE guid = ?";



const QString DatabaseConstants::NOTE_TAGS_TABLE="notes_tags";
const QString DatabaseConstants::CREATE_NOTE_TAGS_TABLE_QUERY = CREATE_TABLE+NOTE_TAGS_TABLE + "(noteGuid TEXT, tagGuid TEXT)";
const QString DatabaseConstants::INSERT_NOTE_TAG_QUERY = INSERT_REPLACE+NOTE_TAGS_TABLE+" (noteGuid, tagGuid) VALUES(?,?)";
const QString DatabaseConstants::SELECT_TAGS_FOR_NOTE = "SELECT * FROM "+NOTE_TAGS_TABLE+" WHERE noteGuid = ?";
const QString DatabaseConstants::DELETE_NOTE_TAGS = "DELETE FROM "+NOTE_TAGS_TABLE+ " WHERE noteGuid = ?";


const QString DatabaseConstants::RESOURCES_TABLE = "resources";

/*

  Guid guid;
  Guid noteGuid;
  Data data;
  std::string mime;
  int16_t width;
  int16_t height;
  int16_t duration;
  bool active;
  Data recognition;
  ResourceAttributes attributes;
  int32_t updateSequenceNum;
  Data alternateData;

*/


const QString DatabaseConstants::CREATE_RESOURCES_TABLE = CREATE_TABLE+RESOURCES_TABLE +"("+
        "guid TEXT PRIMARY KEY,"+
        "noteGuid TEXT,"+
        "dataHash TEXT,"+
        "dataLength NUMBER,"+
        "mime TEXT,"+
        "attachment INTEGER,"+
        "filename TEXT"
        +")";
const QString DatabaseConstants::INSERT_RESOURCE = INSERT_REPLACE+RESOURCES_TABLE+
        " (guid ,"+
        "noteGuid ,"+
        "dataHash ,"+
        "dataLength ,"+
        "mime ,"+
        "attachment ,"+
        "filename "
        +") VALUES (?,?,?,?,?,?,?)";
const QString DatabaseConstants::SELECT_RESOURCES_FOR_NOTE = "SELECT * FROM "+RESOURCES_TABLE+ " WHERE noteGuid=?";
const QString DatabaseConstants::DELETE_NOTE_RESOURCES = "DELETE FROM "+RESOURCES_TABLE+" WHERE noteGuid=?";


DatabaseConstants::DatabaseConstants(QObject *parent) :
    QObject(parent)
{
}
