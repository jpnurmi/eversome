//#define QT_NO_DEBUG_OUTPUT

#include "authenticator.h"
#include "settings.h"
#include <QtConcurrentRun>
#include <QtDebug>
#include "thrift/transport/THttpClient.h"
#include "thrift/protocol/TBinaryProtocol.h"

using namespace evernote::edam;
using namespace apache::thrift;
using namespace apache::thrift::protocol;
using namespace apache::thrift::transport;

static const std::string CONSUMER_KEY = "everel";
static const std::string CONSUMER_SECRET = "201d20eb3ee1f74d";
static const std::string EDAM_HOST = "www.evernote.com";
static const std::string EDAM_ROOT = "/edam/user";
static const int EDAM_PORT = 80;

Authenticator::Authenticator(QObject *parent) : QObject(parent), client(0)
{
    transport = boost::shared_ptr<TTransport>(new THttpClient(EDAM_HOST, EDAM_PORT, EDAM_ROOT));
    client = new UserStoreClient(boost::shared_ptr<TProtocol>(new TBinaryProtocol(transport)));
}

Authenticator::~Authenticator()
{
    delete client;
}

void Authenticator::auth(const QString& username, const QString& password)
{
    qDebug() << Q_FUNC_INFO << username << password;
    QtConcurrent::run(this, &Authenticator::authImpl, username, password);
}

void Authenticator::authImpl(const QString& username, const QString& password)
{
    qDebug() << Q_FUNC_INFO << username << password;
    try {
        if (!transport->isOpen())
            transport->open();
        AuthenticationResult result;
        client->authenticate(result, username.toStdString(), password.toStdString(), CONSUMER_KEY, CONSUMER_SECRET);
        Settings::setValue(Settings::Username, username);
        Settings::setValue(Settings::Password, password);
        Settings::setValue(Settings::AuthToken, QString::fromStdString(result.authenticationToken));
        Settings::setValue(Settings::UserShardID, QString::fromStdString(result.user.shardId));
        qDebug() << Q_FUNC_INFO << "SUCCEED";
        emit succeed();
    } catch (EDAMUserException& e) {
        QString error;
        if (e.errorCode == INVALID_AUTH) {
            if (e.parameter == "password")
                error = tr("__invalid_password__");
            else if (e.parameter == "username")
                error = tr("__invalid_username__");
        }
        if (error.isEmpty())
            error = tr("__unknown_error__");
        qDebug() << Q_FUNC_INFO << "FAILED" << error;
        emit failed(error);
    } catch (TException& e) {
        qDebug() << Q_FUNC_INFO << e.what();
        emit failed(tr("__unknown_error__"));
    }
}
