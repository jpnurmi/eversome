//#define QT_NO_DEBUG_OUTPUT

#include "userstore.h"
#include "settings.h"
#include "manager.h"
#include <QtConcurrentRun>
#include <QtDebug>
#include "thrift/transport/THttpClient.h"
#include "thrift/protocol/TBinaryProtocol.h"

using namespace boost;
using namespace apache;
using namespace evernote;

UserStore::UserStore(QObject *parent) : QObject(parent), active(false)
{
}

UserStore::~UserStore()
{
}

bool UserStore::isActive() const
{
    return active;
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
    qDebug() << Q_FUNC_INFO << un << QString(pw.length(), 'x');
    QtConcurrent::run(this, &UserStore::loginImpl, un, pw, remember);
}

void UserStore::logout()
{
    qDebug() << Q_FUNC_INFO;
    Settings::reset();
    emit loggedOut();
}

void UserStore::loginImpl(const QString& username, const QString& password, bool remember)
{
    qDebug() << Q_FUNC_INFO << username << QString(password.length(), 'x');

    active = true;
    emit isActiveChanged();

    QString err;
    try {
        QString host = Settings::value(Settings::Hostname);
        int port = Settings::value(Settings::ServerPort).toInt();

        shared_ptr<thrift::transport::TTransport> transport(new thrift::transport::THttpClient(host.toStdString(), port, "/edam/user"));
        shared_ptr<thrift::protocol::TProtocol> protocol(new thrift::protocol::TBinaryProtocol(transport));
        transport->open();

        edam::AuthenticationResult result;
        QString key = Settings::value(Settings::ConsumerKey);
        QString secret = Settings::value(Settings::ConsumerSecret);

        edam::UserStoreClient client(protocol);
        client.authenticate(result, username.toStdString(), password.toStdString(), key.toStdString(), secret.toStdString());

        if (remember) {
            Settings::setValue(Settings::Username, username);
            Settings::setValue(Settings::Password, password);
        }

        Settings::setValue(Settings::AuthToken, QString::fromStdString(result.authenticationToken));
        Settings::setValue(Settings::UserShardID, QString::fromStdString(result.user.shardId));

        emit loggedIn();

    } catch (edam::EDAMUserException& e) {
        err = Manager::errorString(e.errorCode);
    } catch (edam::EDAMSystemException& e) {
        err = Manager::errorString(e.errorCode);
    } catch (thrift::TException& e) {
        err = QString::fromUtf8(e.what());
    }

    if (!err.isEmpty()) {
        qDebug() << Q_FUNC_INFO << err;
        emit error(err);
    }

    active = false;
    emit isActiveChanged();
}
