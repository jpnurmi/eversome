#ifndef DATABASE_H
#define DATABASE_H

#include <QVector>
#include <QVariant>
#include "edam/Types_types.h"

class Database
{
public:
    static bool initialize();
    static void uninitialize();

    static bool reset();

    static void beginTransaction();
    static void commitTransaction();

    static bool saveTag(const evernote::edam::Tag& tag);
    static QVector<evernote::edam::Tag> getTags();

    static bool saveNotebook(const evernote::edam::Notebook& notebook);
    static QVector<evernote::edam::Notebook> getNotebooks();

    static bool saveNote(const evernote::edam::Note& note);
    static bool deleteNote(const evernote::edam::Note& note);
    static QVector<evernote::edam::Note> getNotes();

    static QVector<std::string> getNoteTagGuids(const evernote::edam::Note& note);
    static QVector<evernote::edam::Resource> getNoteResources(const evernote::edam::Note& note);

private:
    static bool saveNoteTags(const evernote::edam::Note& note);
    static bool deleteNoteTags(const evernote::edam::Note& note);

    static bool saveNoteResources(const evernote::edam::Note& note);
    static bool deleteNoteResources(const evernote::edam::Note& note);
};

#endif // DATABASEMANAGER_H
