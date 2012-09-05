/*
* Copyright (C) 2012 J-P Nurmi <jpnurmi@gmail.com>
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*/
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
        case Settings::ServerUSN: return QLatin1String("server_usn");
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

void Settings::reset()
{
    QSettings settings;
    settings.remove(keyToString(Username));
    settings.remove(keyToString(Password));
    settings.remove(keyToString(ServerUSN));
}
