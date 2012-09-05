//#define QT_NO_DEBUG_OUTPUT

#include "userstore.h"
#include "settings.h"
#include "manager.h"
#include <QtConcurrentRun>
#include <QReadWriteLock>
#include <QWriteLocker>
#include <QReadLocker>
#include <QtDebug>
#include "edam/UserStore_constants.h"
#include "thrift/transport/THttpClient.h"
#include "thrift/protocol/TBinaryProtocol.h"

static QReadWriteLock lock;

using namespace boost;
using namespace apache;
using namespace evernote;

UserStore::UserStore(QObject *parent) : QObject(parent), active(false)
{
}

UserStore::~UserStore()
{
}

QString UserStore::authToken() const
{
    QReadLocker locker(&lock);
    return token;
}

QString UserStore::notesUrl() const
{
    QReadLocker locker(&lock);
    return notes;
}

QDateTime UserStore::currentTime() const
{
    QReadLocker locker(&lock);
    return time;
}

QDateTime UserStore::expiration() const
{
    QReadLocker locker(&lock);
    return expires;
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

        edam::UserStoreClient client(protocol);
        if (!client.checkVersion("EverN9/0.0.3; MeeGo/Harmattan 1.2", // TODO: hardcoded values
                                 edam::g_UserStore_constants.EDAM_VERSION_MAJOR,
                                 edam::g_UserStore_constants.EDAM_VERSION_MINOR)) {
            QString err = Manager::errorString(Manager::TooOldProtocol);
            err = err.arg(edam::g_UserStore_constants.EDAM_VERSION_MAJOR)
                     .arg(edam::g_UserStore_constants.EDAM_VERSION_MINOR);
            throw thrift::TException(err.toStdString());
        }

        edam::AuthenticationResult result;
        QString key = Settings::value(Settings::ConsumerKey);
        QString secret = Settings::value(Settings::ConsumerSecret);
        client.authenticate(result, username.toStdString(), password.toStdString(), key.toStdString(), secret.toStdString());

        if (remember) {
            Settings::setValue(Settings::Username, username);
            Settings::setValue(Settings::Password, password);
        }

        QWriteLocker locker(&lock);
        token = QString::fromStdString(result.authenticationToken);
        notes = QString::fromStdString(result.noteStoreUrl);
        time = QDateTime::fromMSecsSinceEpoch(result.currentTime);
        expires = QDateTime::fromMSecsSinceEpoch(result.expiration);
        locker.unlock();

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
