#include "resourceitem.h"
#include "resourcemodel.h"
#include <QDesktopServices>
#include <QHash>
#include <QDir>

static void fill_file_extensions();
typedef QHash<QString, QString> StringHash;
Q_GLOBAL_STATIC_WITH_INITIALIZER(StringHash, file_extensions, fill_file_extensions())

void fill_file_extensions()
{
    file_extensions()->insert("application/msword", "doc");
    file_extensions()->insert("application/vnd.openxmlformats-officedocument.wordprocessingml.document", "docx");
    file_extensions()->insert("application/pdf", "pdf");
    file_extensions()->insert("audio/amr", "amr");
    file_extensions()->insert("audio/mpeg", "mp3");
    file_extensions()->insert("audio/wav", "wav");
    file_extensions()->insert("audio/vnd.wave", "wav");
    file_extensions()->insert("image/bmp", "bmp");
    file_extensions()->insert("image/gif", "gif");
    file_extensions()->insert("image/jpg", "jpg");
    file_extensions()->insert("image/jpeg", "jpg");
    file_extensions()->insert("image/pjpeg", "jpg");
    file_extensions()->insert("image/png", "png");
    file_extensions()->insert("text/plain", "txt");
    file_extensions()->insert("text/html", "html");
}

ResourceItem::ResourceItem(evernote::edam::Resource resource, QObject* parent)
    : QObject(parent), m_resource(resource)
{
    qRegisterMetaType<ResourceItem*>();
}

ResourceItem::~ResourceItem()
{
}

evernote::edam::Resource ResourceItem::resource() const
{
    return m_resource;
}

QString ResourceItem::guid() const
{
    return QString::fromStdString(m_resource.guid);
}

QString ResourceItem::filePath() const
{
    QString ext = file_extensions()->value(QString::fromStdString(m_resource.mime));
    QDir dir(QDesktopServices::storageLocation(QDesktopServices::DataLocation));
    return dir.absoluteFilePath(guid() + "." + ext);
}
