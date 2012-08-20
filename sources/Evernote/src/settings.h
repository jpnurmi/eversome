#ifndef SETTINGS_H
#define SETTINGS_H

#include <QObject>
#include "edam/UserStore.h"
#include "edam/UserStore_constants.h"
#include "edam/NoteStore.h"
#include "edam/NoteStore_constants.h"
#include "edam/NoteStore_types.h"
#include "db/databaseconstants.h"
#include "db/databasemanager.h"

using namespace evernote::edam;

class Settings : public QObject
{
    Q_OBJECT
public:
    static Settings* instance();
    Settings(QObject *parent = 0);
    ~Settings();

signals:

public slots:
    static void drop();

    void setPassword(const QString& password);
    QString getPassword();

    void setUsername(const QString& username);
    QString getUsername();

    bool areCredentialsSaved();


    void setAuthToken(const QString& token);
    QString getAuthToken();

    void setUser(User user);
    User getUser();

private:
     static Settings* m_instance;
};

#endif // SETTINGS_H
