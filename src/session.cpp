#include <QDebug>

#include <QtCore>
#include "session.h"
#include "fileutils.h"
#include "database.h"

static const std::string CONSUMER_KEY = "everel";
static const std::string CONSUMER_SECRET = "201d20eb3ee1f74d";
static const std::string EDAM_HOST = "www.evernote.com";
static const std::string EDAM_USER_ROOT = "/edam/user";
static const std::string EDAM_NOTE_ROOT = "/edam/note/";

Session::Session(QObject *parent) :
    QObject(parent)
{
    Database::initialize();
    userStoreClient = NULL;
    syncClient = NULL;
    syncInProgress = false;
    syncCancelled = false;
    cancelGetNote = false;
}

Session::~Session()
{
    if(userStoreClient){
        qDebug() << "Session :: free UserStore client" << endl;
        delete userStoreClient;
    }
    Database::uninitialize();
}

Session* Session::instance(){
    static Session session;
    return &session;
}

void Session::logout(){
    if(syncInProgress){
        return;
    }
    logoutStarted();
    cancelSync();
    Settings::reset();
    Database::reset();
    Cache::instance()->clear();
    Cache::instance()->clearFileCache();
    logoutFinished();
}
void Session::logoutAsync(){
    if(syncInProgress){
        return;
    }
    QtConcurrent::run(this, &Session::logout);
}

void Session::exit(){
    qDebug() << "Session :: exit" << endl;
    if(userStoreTransport){
        if(userStoreTransport->isOpen()){
            qDebug () << "Session :: close UserStore transport... ";
            userStoreTransport->close();
            qDebug () << "closed" << endl;
        }else{
            qDebug() << "Session :: UserStore transport already closed" << endl;
        }
    }

}

void Session::recreateUserStoreClient(bool force){
    if(force){
        if(userStoreTransport != NULL){
            if(userStoreTransport->isOpen()){
                userStoreTransport->close();
            }
        }
        if(userStoreClient != NULL){
            delete userStoreClient;
            userStoreClient = NULL;
        }
    }
    if(userStoreClient == NULL){
        userStoreTransport = shared_ptr<TTransport> (new THttpClient(EDAM_HOST,80,EDAM_USER_ROOT));
        shared_ptr<TProtocol> protocol(new TBinaryProtocol(userStoreTransport));
        userStoreClient = new UserStoreClient(protocol);
    }
    if(!userStoreTransport->isOpen()){
        userStoreTransport->open();
    }
}
void Session::recreateSyncClient(bool force){
    if(force){
        if(syncTransport != NULL){
            if(syncTransport->isOpen()){
                syncTransport->close();
            }

        }
        if(syncClient != NULL){
            delete syncClient;
            syncClient = NULL;
        }
    }
    if(syncClient == NULL){
        User user;
        user.shardId = Settings::value(Settings::UserShardID).toStdString();
        syncTransport = shared_ptr<TTransport> (new THttpClient(EDAM_HOST,80,EDAM_NOTE_ROOT+user.shardId));
        shared_ptr<TProtocol> protocol(new TBinaryProtocol(syncTransport));
        syncClient = new NoteStoreClient(protocol);
    }
    if(!syncTransport->isOpen()){
        syncTransport->open();
    }
}
void Session::getNoteContent(NoteWrapper* note){
    qDebug() << "Session :: auth" << endl;
    noteLoadStarted(note);
    try {
        note->note.tagGuids = Database::getNoteTagGuids(note->note).toStdVector();
        note->note.resources = Database::getNoteResources(note->note).toStdVector();


        if(!FileUtils::noteCached(note)){
            recreateSyncClient(false);
            std::string content = "";
            syncClient->getNoteContent(content, Settings::value(Settings::AuthToken).toStdString(),note->getGuid());
            FileUtils::cacheNoteContent(note, QString::fromStdString(content));
        }
        if(cancelGetNote){
            return;
        }
        noteContentDownloaded(/*FileUtils::noteContentFilePath(note)*/note);
        sleep(1);
        for(int i=0;i<note->note.resources.size();i++){
            Resource r = note->note.resources.at(i);
            if(!FileUtils::resourceCached(r)){
                recreateSyncClient(false);
                syncClient->getResource(r, Settings::value(Settings::AuthToken).toStdString(),r.guid, true, false, true, false);
                FileUtils::cacheResourceContent(r);
                r.data.bodyHash = ResourceWrapper::convertToHex(r.data.bodyHash).toStdString();
            }
            if(cancelGetNote){
                return;
            }
            ResourceWrapper* w = new ResourceWrapper(r);
            resourceDownloaded(w);
        }

        noteLoadFinished(note);
    } catch (TException &tx) {
        qDebug() << "Session :: exception while getNoteContent: " << tx.what();
        if(!cancelGetNote){
            noteLoadError(QString::fromAscii(tx.what()));
        }else{
            qDebug() << "note load canceled, supress errors";
        }
    }
}
void Session::getNoteContentAsync(NoteWrapper* note){
    cancelGetNote = false;
    QtConcurrent::run(this, &Session::getNoteContent, note);
}
void Session::cancelGetNoteContent(){
    cancelGetNote = true;
    try{
        if(syncTransport != NULL){
            syncTransport->close();
        }
        qDebug() << "close transport";
    }catch(TException& e){
        qDebug() << "exception while closing transport: " << QString::fromAscii(e.what());
    }
}

void Session::authAsync(const QString& username, const QString& password){
    QtConcurrent::run(this, &Session::auth, username, password);
}

void Session::auth(const QString& username, const QString& password){
    qDebug() << "Session :: auth" << endl;
    try {
        recreateUserStoreClient(true);
        AuthenticationResult result;
        userStoreClient->authenticate(result,username.toStdString(),password.toStdString(),CONSUMER_KEY,CONSUMER_SECRET);
        qDebug() << "Session :: got auth token " << result.authenticationToken.c_str();
        Settings::setValue(Settings::Username, username);
        Settings::setValue(Settings::Password, password);
        Settings::setValue(Settings::AuthToken, QString::fromStdString(result.authenticationToken));
        Settings::setValue(Settings::UserShardID, QString::fromStdString(result.user.shardId));
        recreateSyncClient(true);
        authenticationSuccess();
    }catch (EDAMUserException& e){
        if(e.errorCode == DATA_REQUIRED){
            if(e.parameter == "password"){
                authenticationFailed(tr("__empty_password__"));
            }else if(e.parameter == "username"){
                authenticationFailed(tr("__empty_username__"));
            }
        }else if(e.errorCode == INVALID_AUTH){
            if(e.parameter == "password"){
                authenticationFailed(tr("__invalid_password__"));
            }else if(e.parameter == "username"){
                authenticationFailed(tr("__invalid_username__"));
            }
        }else{
            authenticationFailed(tr("__basic_network_error__"));
        }

    }

    catch (TException &tx) {
        qDebug() << "Session :: excetion while login: " << tx.what();
        authenticationFailed(tr("__basic_network_error__"));
    }
}
void Session::reauth(){
    auth(Settings::value(Settings::Username), Settings::value(Settings::Password));
}

void Session::sync(){
    if(syncInProgress){
        return;
    }
    syncInProgress = true;
    syncCancelled = false;

    try{
        for(int i=0;i<5;i++){
            try{

                recreateUserStoreClient(false);
                recreateSyncClient(false);

                qDebug() << "Session :: start sync...";
                int cacheUsn = Settings::value(Settings::ServerUSN).toInt();
                qDebug() << "Session :: saved USN: " << cacheUsn;
                SyncChunk chunk;
                int percent = 0;
                while(true){
                    syncStarted(percent);
                    syncClient->getSyncChunk(chunk, Settings::value(Settings::AuthToken).toStdString(), cacheUsn, 1024, false);

                    if(cacheUsn >= chunk.updateCount){
                        break;
                    }
                    percent = (int)((double)(100* (double)cacheUsn/(double)chunk.updateCount));
                    syncStarted(percent);
                    std::vector <Tag> tags = chunk.tags;

                    if(!tags.empty()){


                        tagsSyncStarted();
                        Database::beginTransaction();
                        for(int i=0;i<tags.size();i++){
                            if(syncCancelled){
                                syncCancelled = false;
                                syncInProgress = false;
                                syncFinished();
                                return;
                            }
                            Tag tag = tags.at(i);
                            Database::saveTag(tag);
                            qDebug() << "Session :: tag " << tag.name.c_str();
                        }
                        Database::commitTransaction();
                    }
                    syncStarted(percent);
                    if(syncCancelled){
                        syncCancelled = false;
                        syncInProgress = false;
                        syncFinished();
                        return;
                    }

                    std::vector <Notebook> notebooks = chunk.notebooks;
                    qDebug() << "Session :: notebooks " << chunk.notebooks.size();
                    if(!notebooks.empty()){


                        notebooksSyncStarted();
                        Database::beginTransaction();
                        for(int i=0;i<notebooks.size();i++){
                            if(syncCancelled){
                                syncCancelled = false;
                                syncInProgress = false;
                                syncFinished();
                                return;
                            }
                            Notebook notebook = notebooks.at(i);
                            Database::saveNotebook(notebook);
                            qDebug() << "Session :: notebook " << notebook.name.c_str();
                        }
                        Database::commitTransaction();
                    }
                    syncStarted(percent);
                    if(syncCancelled){
                        syncCancelled = false;
                        syncInProgress = false;
                        syncFinished();
                        return;
                    }
                    std::vector <Note> notes = chunk.notes;
                    qDebug() << "Session :: notes " << chunk.notes.size();
                    if(!notes.empty()){
                        Database::beginTransaction();
                        for(int i=0;i<notes.size();i++){
                            if(syncCancelled){
                                syncCancelled = false;
                                syncInProgress = false;
                                syncFinished();
                                return;
                            }
                            Note note = notes.at(i);
                            if(note.deleted){
                                Database::deleteNote(note);
                                FileUtils::removeNoteCache(note);
                            }else{
                                Database::saveNote(note);
                            }
                            qDebug() << "Session :: note " << note.title.c_str();
                        }
                        Database::commitTransaction();
                    }
                    syncStarted(percent);

                    qDebug() << "expunged notes: " << chunk.expungedNotes.size();

                    cacheUsn = chunk.chunkHighUSN;
                    Settings::setValue(Settings::ServerUSN, QString::number(cacheUsn));
                    if(cacheUsn >= chunk.updateCount){
                        break;
                    }
                    qDebug() << "Current usn: " << cacheUsn << " high usn: " << chunk.chunkHighUSN << ", update count: " << chunk.updateCount;
                }

                qDebug() << "Session :: sync finished";
                break;
            }catch(EDAMUserException &e){
                if(e.errorCode == 9){
                    reauth();
                }
            }

        }
    }catch(TException &tx){
        qDebug() << "Session :: excetion while sync: " << tx.what();
        syncFailed("Network error");
    }
    syncInProgress = false;
    syncFinished();
    Cache::instance()->load();
}
void Session::syncAsync(){
    qDebug() << "syncAsync called";
    QtConcurrent::run(this, &Session::sync);
}
bool Session::isSyncInProgress(){
    return syncInProgress;
}
void Session::cancelSync(){
    syncCancelled = true;
}
