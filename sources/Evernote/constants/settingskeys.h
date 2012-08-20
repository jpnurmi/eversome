#ifndef SETTINGSKEYS_H
#define SETTINGSKEYS_H

#include <QString>

class SettingsKeys
{

private:
    SettingsKeys();
public:
    static const QString USERNAME;
    static const QString PASSWORD;
    static const QString AUTH_TOKEN;
    static const QString SERVER_USN;
    static const QString USER_SHARDID;
};

#endif // SETTINGSKEYS_H
