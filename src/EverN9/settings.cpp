#include "settings.h"
#include <QSettings>
#include <QVariant>
#include <QFile>

static QString keyToString(Settings::Key key)
{
    switch (key)
    {
        case Settings::Username: return QLatin1String("username");
        case Settings::Password: return QLatin1String("password");
        case Settings::AuthToken: return QLatin1String("auth_token");
        case Settings::ServerUSN: return QLatin1String("server_usn");
        case Settings::UserShardID: return QLatin1String("user_shardid");
        case Settings::ConsumerKey: return QLatin1String("consumer_key");
        case Settings::ConsumerSecret: return QLatin1String("consumer_secret");
        case Settings::Hostname: return QLatin1String("host_name");
        case Settings::ServerPort: return QLatin1String("server_port");
        default: Q_ASSERT(false); return QString();
    }
}

QString Settings::value(Settings::Key key)
{
    return QSettings().value(keyToString(key)).toString();
}

void Settings::setValue(Settings::Key key, const QString& value)
{
    QSettings().setValue(keyToString(key), value);
}

bool Settings::reset()
{
    return QFile::remove(QSettings().fileName());
}
