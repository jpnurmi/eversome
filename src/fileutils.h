#ifndef FILEUTILS_H
#define FILEUTILS_H

#include <QString>
#include "edam/Types_types.h"
#include "wrappers/notewrapper.h"

class FileUtils
{
public:
    static QString noteContentFilePath(NoteWrapper* note);
    static QString myDocsFolderPath();
    static QString noteFolderPath(NoteWrapper* note);
    static QString noteFolderPath(std::string note);
    static QString notesFolderPath();
    static QString appFolderPath();
    static QString resourceContentFilePath(Resource r);
    static QString resourceFolderPath(Resource r);

    static bool noteCached(NoteWrapper* note);
    static void cacheNoteContent(NoteWrapper* note, QString content);
    static bool resourceCached(Resource r);
    static void cacheResourceContent(Resource r);
    static QString getExtension(Resource r);

    static bool removeDir(const QString &dirName);
    static void removeNoteCache(Note note);
    static void removeNoteDir();

    static QString readNoteContent(NoteWrapper* note);
};

#endif // FILEUTILS_H
