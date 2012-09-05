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

UserStore::UserStore(QObject *parent) : QObject(parent),
    loggingIn(false), loggingOut(false), client(0)
{
    QString host = Settings::value(Settings::Hostname);
    int port = Settings::value(Settings::ServerPort).toInt();
    transport = boost::shared_ptr<TTransport>(new THttpClient(host.toStdString(), port, "/edam/user"));
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

void UserStore::login(const QString& username, const QString& password, bool remember)
{
    QString un = username.isEmpty() ? Settings::value(Settings::Username) : username;
    QString pw = password.isEmpty() ? Settings::value(Settings::Password) : password;
    qDebug() << Q_FUNC_INFO << un << pw;
    QtConcurrent::run(this, &UserStore::loginImpl, un, pw, remember);
}

void UserStore::logout()
{
    qDebug() << Q_FUNC_INFO;
    Settings::reset();
    QtConcurrent::run(this, &UserStore::logoutImpl);
}

void UserStore::loginImpl(const QString& username, const QString& password, bool remember)
{
    if (loggingIn)
        return;

    qDebug() << Q_FUNC_INFO << username << password;

    loggingIn = true;
    emit isActiveChanged();

    QString err;
    try {
        if (!transport->isOpen())
            transport->open();
        evernote::edam::AuthenticationResult result;
        QString key = Settings::value(Settings::ConsumerKey);
        QString secret = Settings::value(Settings::ConsumerSecret);
        client->authenticate(result, username.toStdString(), password.toStdString(), key.toStdString(), secret.toStdString());
        if (remember) {
            Settings::setValue(Settings::Username, username);
            Settings::setValue(Settings::Password, password);
        }
        Settings::setValue(Settings::AuthToken, QString::fromStdString(result.authenticationToken));
        Settings::setValue(Settings::UserShardID, QString::fromStdString(result.user.shardId));
        emit loggedIn();
    } catch (evernote::edam::EDAMUserException& e) {
        err = Manager::errorString(e.errorCode);
    } catch (evernote::edam::EDAMSystemException& e) {
        err = Manager::errorString(e.errorCode);
    } catch (TException& e) {
        err = QString::fromUtf8(e.what());
    }

    if (!err.isEmpty()) {
        qDebug() << Q_FUNC_INFO << err;
        emit error(err);
    }

    loggingIn = false;
    emit isActiveChanged();
}

void UserStore::logoutImpl()
{
    if (loggingOut)
        return;

    qDebug() << Q_FUNC_INFO;

    loggingOut = true;
    emit isActiveChanged();

    QString err;
    try {
        if (transport->isOpen())
            transport->close();
        emit loggedOut();
    } catch (TException& e) {
        err = QString::fromUtf8(e.what());
    }

    if (!err.isEmpty()) {
        qDebug() << Q_FUNC_INFO << err;
        emit error(err);
    }

    loggingOut = false;
    emit isActiveChanged();
}
