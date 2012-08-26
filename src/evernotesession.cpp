#include <QDebug>

#include <QtCore>
#include "evernotesession.h"
#include "fileutils.h"
#include "db/database.h"

const std::string EvernoteSession::CONSUMER_KEY = "everel";
const std::string EvernoteSession::CONSUMER_SECRET = "201d20eb3ee1f74d";

EvernoteSession::EvernoteSession(QObject *parent) :
    QObject(parent)
{
    Database::initialize();
    userStoreClient = NULL;
    syncClient = NULL;
    syncInProgress = false;
    syncCancelled = false;
    cancelGetNote = false;
}

EvernoteSession::~EvernoteSession()
{
    if(userStoreClient){
        qDebug() << "EvernoteSession :: free UserStore client" << endl;
        delete userStoreClient;
    }
    Database::uninitialize();
}

EvernoteSession* EvernoteSession::instance(){
    static EvernoteSession session;
    return &session;
}

void EvernoteSession::logout(){
    if(syncInProgress){
        return;
    }
    logoutStarted();
    cancelSync();
    Database::reset();
    Cache::instance()->clear();
    Cache::instance()->clearFileCache();
    logoutFinished();
}
void EvernoteSession::logoutAsync(){
    if(syncInProgress){
        return;
    }
    QtConcurrent::run(this, &EvernoteSession::logout);
}

void EvernoteSession::exit(){
    qDebug() << "EvernoteSession :: exit" << endl;
    if(userStoreTransport){
        if(userStoreTransport->isOpen()){
            qDebug () << "EvernoteSession :: close UserStore transport... ";
            userStoreTransport->close();
            qDebug () << "closed" << endl;
        }else{
            qDebug() << "EvernoteSession :: UserStore transport already closed" << endl;
        }
    }

}

void EvernoteSession::recreateUserStoreClient(bool force){
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
        userStoreTransport = shared_ptr<TTransport> (new THttpClient(Constants::EDAM_HOST,80,Constants::EDAM_USER_ROOT));
        shared_ptr<TProtocol> protocol(new TBinaryProtocol(userStoreTransport));
        userStoreClient = new UserStoreClient(protocol);
    }
    if(!userStoreTransport->isOpen()){
        userStoreTransport->open();
    }
}
void EvernoteSession::recreateSyncClient(bool force){
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
        syncTransport = shared_ptr<TTransport> (new THttpClient(Constants::EDAM_HOST,80,Constants::EDAM_NOTE_ROOT+user.shardId));
        shared_ptr<TProtocol> protocol(new TBinaryProtocol(syncTransport));
        syncClient = new NoteStoreClient(protocol);
    }
    if(!syncTransport->isOpen()){
        syncTransport->open();
    }
}
void EvernoteSession::getNoteContent(NoteWrapper* note){
    qDebug() << "EvernoteSession :: auth" << endl;
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
        qDebug() << "EvernoteSession :: exception while getNoteContent: " << tx.what();
        if(!cancelGetNote){
            noteLoadError(QString::fromAscii(tx.what()));
        }else{
            qDebug() << "note load canceled, supress errors";
        }
    }
}
void EvernoteSession::getNoteContentAsync(NoteWrapper* note){
    cancelGetNote = false;
    QtConcurrent::run(this, &EvernoteSession::getNoteContent, note);
}
void EvernoteSession::cancelGetNoteContent(){
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

void EvernoteSession::authAsync(const QString& username, const QString& password){
    QtConcurrent::run(this, &EvernoteSession::auth, username, password);
}

void EvernoteSession::auth(const QString& username, const QString& password){
    qDebug() << "EvernoteSession :: auth" << endl;
    try {
        recreateUserStoreClient(true);
        AuthenticationResult result;
        userStoreClient->authenticate(result,username.toStdString(),password.toStdString(),CONSUMER_KEY,CONSUMER_SECRET);
        qDebug() << "EvernoteSession :: got auth token " << result.authenticationToken.c_str();
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
        qDebug() << "EvernoteSession :: excetion while login: " << tx.what();
        authenticationFailed(tr("__basic_network_error__"));
    }
}
void EvernoteSession::reauth(){
    auth(Settings::value(Settings::Username), Settings::value(Settings::Password));
}

void EvernoteSession::sync(){
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

                qDebug() << "EvernoteSession :: start sync...";
                int cacheUsn = Settings::value(Settings::ServerUSN).toInt();
                qDebug() << "EvernoteSession :: saved USN: " << cacheUsn;
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
                            qDebug() << "EvernoteSession :: tag " << tag.name.c_str();
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
                    qDebug() << "EvernoteSession :: notebooks " << chunk.notebooks.size();
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
                            qDebug() << "EvernoteSession :: notebook " << notebook.name.c_str();
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
                    qDebug() << "EvernoteSession :: notes " << chunk.notes.size();
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
                            qDebug() << "EvernoteSession :: note " << note.title.c_str();
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

                qDebug() << "EvernoteSession :: sync finished";
                break;
            }catch(EDAMUserException &e){
                if(e.errorCode == 9){
                    reauth();
                }
            }

        }
    }catch(TException &tx){
        qDebug() << "EvernoteSession :: excetion while sync: " << tx.what();
        syncFailed("Network error");
    }
    syncInProgress = false;
    syncFinished();
    Cache::instance()->load();
}
void EvernoteSession::syncAsync(){
    qDebug() << "syncAsync called";
    QtConcurrent::run(this, &EvernoteSession::sync);
}
bool EvernoteSession::isSyncInProgress(){
    return syncInProgress;
}
void EvernoteSession::cancelSync(){
    syncCancelled = true;
}
