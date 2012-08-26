#include "settings.h"
#include <QSettings>
#include <QVariant>

static QString keyToString(Settings::Key key)
{
    switch (key)
    {
        case Settings::Username: return QLatin1String("username");
        case Settings::Password: return QLatin1String("password");
        case Settings::AuthToken: return QLatin1String("auth_toke");
        case Settings::ServerUSN: return QLatin1String("server_usn");
        case Settings::UserShardID: return QLatin1String("user_shardid");
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
