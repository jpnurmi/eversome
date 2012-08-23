#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QObject>
#include <QtSql>
#include "databaseconstants.h"
#include "constants/settingskeys.h"
#include "edam/UserStore.h"
#include "edam/UserStore_constants.h"
#include "edam/NoteStore.h"
#include "edam/NoteStore_constants.h"
#include "edam/NoteStore_types.h"
#include "wrappers/resourcewrapper.h"
using namespace std;
using namespace evernote::edam;

class DatabaseManager : public QObject
{
    Q_OBJECT

public:
    static const QString DB_NAME;

    static DatabaseManager* instance();
    DatabaseManager(QObject* parent = 0);
    ~DatabaseManager();

signals:
    void dbOpened();
    void dbOpenError();

public slots:
    void close();

    void beginTransacton();
    void commitTransaction();

    bool saveTag(Tag tag);
    QVector<Tag> getTags() const;

    bool saveNotebook(Notebook notebook);
    QVector<Notebook> getNotebooks() const;

    bool saveNote(Note note);
    void deleteNote(Note note);
    QVector<Note> getNotes() const;

    bool saveResources(Note note);
    void deleteNoteResources(Note note);

    bool saveNoteTags(Note note);
    void deleteNoteTags(Note note);

    std::vector <std::string> getNoteTagGuids(Note note);
    std::vector <Resource> getNoteResources(Note note);

    void createTables();
    void makeSetting(const QString& key, const QString& value);
    void makeIntSetting(const QString& key, int value);

    QString getSetting(const QString& key);
    int getIntSetting(const QString& key);
    void dropTable(const QString& tableName);
    void clear();
};

#endif // DATABASEMANAGER_H
