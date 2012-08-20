#include "settings.h"

Settings* Settings::m_instance = NULL;

Settings::Settings(QObject *parent) :
    QObject(parent)
{

}

Settings* Settings::instance(){
   if(!m_instance){
        m_instance = new Settings();
   }
   return m_instance;
}

void Settings::drop(){
    if(m_instance){
        delete m_instance;
        m_instance = 0;
    }
}
Settings::~Settings() {

}

void Settings::setPassword(const QString &password){
    DatabaseManager::instance()->makeSetting(SettingsKeys::PASSWORD, password);
}
QString Settings::getPassword(){
    return DatabaseManager::instance()->getSetting(SettingsKeys::PASSWORD);
}

void Settings::setUsername(const QString &username){
    DatabaseManager::instance()->makeSetting(SettingsKeys::USERNAME, username);
}
QString Settings::getUsername(){
    return DatabaseManager::instance()->getSetting(SettingsKeys::USERNAME);
}
bool Settings::areCredentialsSaved(){
    QString username = getUsername();
    QString password = getPassword();
    qDebug() << "Creds saved: " << (!username.isEmpty() && !password.isEmpty());
    return !username.isEmpty() && !password.isEmpty();
}
void Settings::setAuthToken(const QString &token){
    DatabaseManager::instance()->makeSetting(SettingsKeys::AUTH_TOKEN, token);
}
QString Settings::getAuthToken(){
    return DatabaseManager::instance()->getSetting(SettingsKeys::AUTH_TOKEN);
}
void Settings::setUser(User user){
    DatabaseManager::instance()->makeSetting(SettingsKeys::USER_SHARDID, user.shardId.c_str());
}
User Settings::getUser(){
    User user;
    user.shardId = DatabaseManager::instance()->getSetting(SettingsKeys::USER_SHARDID).toStdString();
    return user;
}

