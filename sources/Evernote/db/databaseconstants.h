#ifndef DATABASECONSTANTS_H
#define DATABASECONSTANTS_H

#include <QObject>

class DatabaseConstants : public QObject
{
    Q_OBJECT
public:
    explicit DatabaseConstants(QObject *parent = 0);
    //Queries
    static const QString CREATE_TABLE;
    static const QString INSERT_REPLACE;
    static const QString DROP_TABLE_QUERY;
    //Tables
    static const QString SETTINGS_TABLE;
    static const QString CREATE_SETTINGS_TABLE_QUERY;
    static const QString CREATE_UPDATE_SETTING_QUERY;
    static const QString GET_SETTING_QUERY;

    static const QString TAGS_TABLE;
    static const QString CREATE_TAGS_TABLE_QUERY;
    static const QString INSERT_TAG_QUERY;
    static const QString SELECT_TAGS_QUERY;

    static const QString NOTEBOOKS_TABLE;
    static const QString CREATE_NOTEBOOKS_TABLE_QUERY;
    static const QString INSERT_NOTEBOOK_QUERY;
    static const QString SELECT_NOTEBOOKS_QUERY;

    static const QString NOTES_TABLE;
    static const QString CREATE_NOTES_TABLE_QUERY;
    static const QString INSERT_NOTE_QUERY;
    static const QString SELECT_ALL_NOTES_QUERY;
    static const QString DELETE_NOTE;

    static const QString NOTE_TAGS_TABLE;
    static const QString CREATE_NOTE_TAGS_TABLE_QUERY;
    static const QString INSERT_NOTE_TAG_QUERY;
    static const QString SELECT_TAGS_FOR_NOTE;
    static const QString DELETE_NOTE_TAGS;

    static const QString RESOURCES_TABLE;
    static const QString CREATE_RESOURCES_TABLE;
    static const QString INSERT_RESOURCE;
    static const QString SELECT_RESOURCES_FOR_NOTE;
    static const QString DELETE_NOTE_RESOURCES;

signals:

public slots:

};

#endif // DATABASECONSTANTS_H
