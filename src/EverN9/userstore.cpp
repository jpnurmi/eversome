//#define QT_NO_DEBUG_OUTPUT

#include "userstore.h"
#include "settings.h"
#include "manager.h"
#include <QtConcurrentRun>
#include <QtDebug>
#include "thrift/transport/THttpClient.h"
#include "thrift/protocol/TBinaryProtocol.h"

using namespace apache::thrift;
using namespace apache::thrift::protocol;
using namespace apache::thrift::transport;

static const std::string CONSUMER_KEY = "everel";
static const std::string CONSUMER_SECRET = "201d20eb3ee1f74d";
static const std::string EDAM_HOST = "www.evernote.com";
static const std::string EDAM_ROOT = "/edam/user";
static const int EDAM_PORT = 80;

UserStore::UserStore(QObject *parent) : QObject(parent),
    loggingIn(false), loggingOut(false), client(0)
{
    transport = boost::shared_ptr<TTransport>(new THttpClient(EDAM_HOST, EDAM_PORT, EDAM_ROOT));
    client = new evernote::edam::UserStoreClient(boost::shared_ptr<TProtocol>(new TBinaryProtocol(transport)));
}

UserStore::~UserStore()
{
    delete client;
}

bool UserStore::isActive() const
{
    return loggingIn || loggingOut;
}

bool UserStore::hasCredentials() const
{
    return !Settings::value(Settings::Username).isEmpty() &&
           !Settings::value(Settings::Password).isEmpty();
}

void UserStore::login(const QString& username, const QString& password)
{
    QString un = username.isEmpty() ? Settings::value(Settings::Username) : username;
    QString pw = password.isEmpty() ? Settings::value(Settings::Password) : password;
    qDebug() << Q_FUNC_INFO << un << pw;
    QtConcurrent::run(this, &UserStore::loginImpl, un, pw);
}

void UserStore::logout()
{
    qDebug() << Q_FUNC_INFO;
    QtConcurrent::run(this, &UserStore::logoutImpl);
}

void UserStore::loginImpl(const QString& username, const QString& password)
{
    if (loggingIn)
        return;

    qDebug() << Q_FUNC_INFO << username << password;

    loggingIn = true;
    emit activeChanged();

    QString err;
    try {
        if (!transport->isOpen())
            transport->open();
        evernote::edam::AuthenticationResult result;
        client->authenticate(result, username.toStdString(), password.toStdString(), CONSUMER_KEY, CONSUMER_SECRET);
        Settings::setValue(Settings::Username, username);
        Settings::setValue(Settings::Password, password);
        Settings::setValue(Settings::AuthToken, QString::fromStdString(result.authenticationToken));
        Settings::setValue(Settings::UserShardID, QString::fromStdString(result.user.shardId));
        emit loggedIn();
    } catch (evernote::edam::EDAMUserException& e) {
        err = Manager::errorString(e.errorCode);
    } catch (evernote::edam::EDAMSystemException& e) {
        err = Manager::errorString(e.errorCode);
    } catch (evernote::edam::EDAMNotFoundException& e) {
        err = Manager::errorString(-1);
    } catch (TException& e) {
        err = QString::fromUtf8(e.what());
    }

    if (!err.isEmpty()) {
        qDebug() << Q_FUNC_INFO << err;
        emit error(err);
    }

    loggingIn = false;
    emit activeChanged();
}

void UserStore::logoutImpl()
{
    if (loggingOut)
        return;

    qDebug() << Q_FUNC_INFO;

    loggingOut = true;
    emit activeChanged();

    QString err;
    try {
        if (transport->isOpen())
            transport->close();
        emit loggedOut();
    } catch (evernote::edam::EDAMUserException& e) {
        err = Manager::errorString(e.errorCode);
    } catch (evernote::edam::EDAMSystemException& e) {
        err = Manager::errorString(e.errorCode);
    } catch (evernote::edam::EDAMNotFoundException& e) {
        err = Manager::errorString(-1);
    } catch (TException& e) {
        err = QString::fromUtf8(e.what());
    }

    if (!err.isEmpty()) {
        qDebug() << Q_FUNC_INFO << err;
        emit error(err);
    }

    loggingOut = false;
    emit activeChanged();
}
