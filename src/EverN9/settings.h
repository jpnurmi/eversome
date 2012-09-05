#ifndef SETTINGS_H
#define SETTINGS_H

#include <QString>

class Settings
{
public:
    enum Key
    {
        Username,
        Password,
        AuthToken,
        ServerUSN,
        UserShardID,
        ConsumerKey,
        ConsumerSecret,
        Hostname,
        ServerPort
    };

    static QString value(Key key);
    static void setValue(Key key, const QString& value);

    static void reset();
};

#endif // SETTINGS_H
