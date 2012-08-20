#ifndef EVERNOTESESSION_H
#define EVERNOTESESSION_H
#include "edam/UserStore.h"
#include "constants/constants.h"
#include "cache.h"
#include "thrift/transport/THttpClient.h"
#include "thrift/protocol/TBinaryProtocol.h"
#include "thrift/transport/TSSLSocket.h"
#include "thrift/Thrift.h"
#include "edam/UserStore.h"
#include "edam/UserStore_constants.h"
#include "edam/NoteStore.h"
#include "edam/NoteStore_constants.h"
#include "edam/NoteStore_types.h"
#include "edam/Errors_constants.h"
#include "edam/Errors_types.h"
#include "db/databasemanager.h"
#include "db/databaseconstants.h"
#include "constants/settingskeys.h"
#include <boost/shared_ptr.hpp>
#include <QObject>
#include "settings.h"
#include "fileutils.h"
#include "wrappers/resourcewrapper.h"

using namespace std;
using namespace boost;

using namespace apache::thrift::transport;
using namespace apache::thrift::protocol;
using namespace apache::thrift;


using namespace evernote::edam;


class EvernoteSession : public QObject
{
    Q_OBJECT
public:
    static const std::string CONSUMER_KEY;
    static const std::string CONSUMER_SECRET;

    static EvernoteSession* instance();
    EvernoteSession(QObject *parent = 0);
    ~EvernoteSession();



    void recreateUserStoreClient(bool force);
    void recreateSyncClient(bool force);
signals:
    void authenticationSuccess();
    void authenticationFailed(QString error);

    void syncFailed(QString error);
    void syncStarted(int percent);
    void syncFinished();

    void tagsSyncStarted();
    void notebooksSyncStarted();
    void noteLoadStarted(NoteWrapper* note);
    void noteLoadFinished(NoteWrapper* note);
    void noteLoadError(QString error);
    void noteContentDownloaded(NoteWrapper* note);
    void resourceDownloaded(ResourceWrapper* r);
    void logoutStarted();
    void logoutFinished();

public slots:
    void reauth();
    void auth(const QString& username, const QString& password);
    void authAsync(const QString& username, const QString& password);

    void sync();
    void syncAsync();

    void getNoteContentAsync(NoteWrapper* note);
    void getNoteContent(NoteWrapper* note);


    void exit();
    static void drop();
    void logout();
    void logoutAsync();
    bool isSyncInProgress();
    void cancelGetNoteContent();
    void cancelSync();
private:
    static EvernoteSession* m_instance;
    UserStoreClient* userStoreClient;
    shared_ptr<TTransport> userStoreTransport;
    NoteStoreClient* syncClient;
    shared_ptr<TTransport> syncTransport;



    bool syncInProgress;
    bool cancelGetNote;
    bool syncCancelled;

};

#endif // EVERNOTESESSION_H
