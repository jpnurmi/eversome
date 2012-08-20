#include "fileutils.h"

const QString FileUtils::NOTES_FOLDER = "notes";
const QString FileUtils::APP_FOLDER = ".evernote";
const QString FileUtils::MY_DOCS_FOLDER = "/home/user/MyDocs";
const QString FileUtils::CONTENT_FILENAME = "content.html";


FileUtils::FileUtils(QObject *parent) :
    QObject(parent)
{
}

void FileUtils::cacheNoteContent(NoteWrapper *note, QString content){
    QDir mydocs(myDocsFolderPath());
    mydocs.mkdir(APP_FOLDER);

    QDir appDir(appFolderPath());
    appDir.mkdir(NOTES_FOLDER);

    QDir notesFolder(notesFolderPath());
    QString noteFolderName = QString::fromStdString(note->getGuid());
    notesFolder.mkdir(noteFolderName);

    QDir noteFolder(noteFolderPath(note));

    QString contentFileName = noteFolder.filePath(CONTENT_FILENAME);

    qDebug() << "content file name: " << contentFileName;

    QFile contentFile(contentFileName);

    contentFile.open(QIODevice::WriteOnly | QIODevice::Text);

    QTextStream out(&contentFile);
    out << "<html>";
    out.flush();

    out << "<head>";
    out << "<meta http-equiv='Content-Type' content='text/html; charset=UTF-8' /> ";
    out << "<script src = '/opt/Evernote/qml/Evernote/engine.js' type='text/javascript' language='JavaScript'></script></head>";
    out.flush();

    out << "<body onload='onLoadHandler();' onclick='onClickHandler(event);' >";
    out.flush();

    out << content;
    out.flush();

    out << "</body></html>";
    contentFile.flush();
    contentFile.close();
}

void FileUtils::cacheResourceContent(Resource r){
    QDir noteFolder(noteFolderPath(r.noteGuid));
    noteFolder.mkdir(QString::fromStdString(r.guid));

    QDir resourceFolder(resourceFolderPath(r));
    QFile contentFile(resourceFolder.filePath(QString::fromStdString(r.guid)+"."+getExtension(r)));
    contentFile.open(QIODevice::WriteOnly | QIODevice::Text);

    qDebug() << "hash: " << QString::fromStdString(r.data.bodyHash);

    contentFile.write(r.data.body.data(),r.data.size);

    contentFile.flush();
    contentFile.close();
}

QString FileUtils::myDocsFolderPath(){
    return MY_DOCS_FOLDER;
}
QString FileUtils::appFolderPath(){
    return myDocsFolderPath() + QDir::separator() + APP_FOLDER;
}
QString FileUtils::notesFolderPath(){
    return appFolderPath() + QDir::separator() + NOTES_FOLDER;
}
QString FileUtils::noteFolderPath(NoteWrapper *note){
   return noteFolderPath(note->getGuid());
}
QString FileUtils::noteFolderPath(std::string noteGuid){
 return notesFolderPath() + QDir::separator()+QString::fromStdString(noteGuid);
}

QString FileUtils::noteContentFilePath(NoteWrapper *note){
    return noteFolderPath(note) + QDir::separator() + CONTENT_FILENAME;
}

QString FileUtils::resourceFolderPath(Resource r){
    return notesFolderPath() + QDir::separator() + QString::fromStdString(r.noteGuid)+ QDir::separator() + QString::fromStdString(r.guid);
}
QString FileUtils::resourceContentFilePath(Resource r){
    return resourceFolderPath(r) + QDir::separator() + QString::fromStdString(r.guid)+"."+getExtension(r);
}

bool FileUtils::noteCached(NoteWrapper *note){
    QFile contentFile(noteContentFilePath(note));
    return contentFile.exists();
}
bool FileUtils::resourceCached(Resource r){
    QFile resourceContentFile(resourceContentFilePath(r));
    return resourceContentFile.exists();
}

QString FileUtils::getExtension(Resource r){
    if(r.mime == "audio/mpeg"){
        return "mp3";
    }else if(r.mime == "image/jpeg" || r.mime == "image/pjpeg" || r.mime == "image/jpg"){
        return "jpeg";
    }else if(r.mime == "text/plain"){
        return "txt";
    }else if(r.mime == "text/html"){
        return "html";
    }else if(r.mime == "image/png"){
        return "png";
    }else if(r.mime == "image/gif"){
        return "gif";
    }else if(r.mime == "image/bmp"){
        return "bmp";
    }else if(r.mime == "audio/wav" || r.mime == "audio/vnd.wave"){
        return "wav";
    }else if(r.mime == "audio/amr"){
        return "amr";
    }else if(r.mime == "application/pdf"){
        return "pdf";
    }else if(r.mime == "application/msword"){
        return "doc";
    }else if(r.mime == "application/vnd.openxmlformats-officedocument.wordprocessingml.document"){
        return "docx";
    }
    return "";
}
bool FileUtils::removeDir(const QString &dirName)
{
    bool result = true;
    QDir dir(dirName);

    if (dir.exists(dirName)) {
        Q_FOREACH(QFileInfo info, dir.entryInfoList(QDir::NoDotAndDotDot | QDir::System | QDir::Hidden  | QDir::AllDirs | QDir::Files, QDir::DirsFirst)) {
            if (info.isDir()) {
                result = removeDir(info.absoluteFilePath());
            }
            else {
                result = QFile::remove(info.absoluteFilePath());
            }

            if (!result) {
                return result;
            }
        }
        result = dir.rmdir(dirName);
    }

    return result;
}
void FileUtils::removeNoteCache(Note note){
    removeDir(noteFolderPath(note.guid));
}
void FileUtils::removeNoteDir(){
    removeDir(notesFolderPath());
}
QString FileUtils::readNoteContent(NoteWrapper* note){
     QFile contentFile(FileUtils::noteContentFilePath(note));

    contentFile.open(QIODevice::ReadOnly | QIODevice::Text);

    QTextStream in(&contentFile);
    QString result = "";

    while(!in.atEnd()){
        QString line = in.readLine();
        result += line;
    }
    contentFile.close();
    return result;
}
