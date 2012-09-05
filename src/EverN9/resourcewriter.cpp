//#define QT_NO_DEBUG_OUTPUT

#include "resourcewriter.h"
#include <QtConcurrentRun>
#include <QFileInfo>
#include <QVariant>
#include <QFile>
#include <QDir>

ResourceWriter::ResourceWriter(QObject* parent) : QObject(parent), writing(false)
{
}

ResourceWriter::~ResourceWriter()
{
}

bool ResourceWriter::isWriting() const
{
    return writing;
}

void ResourceWriter::write(const QString& filePath, const QByteArray& data)
{
    qDebug() << Q_FUNC_INFO << filePath << data.length();
    QtConcurrent::run(this, &ResourceWriter::writeImpl, filePath, data);
}

void ResourceWriter::writeImpl(const QString& filePath, const QByteArray& data)
{
    qDebug() << Q_FUNC_INFO << filePath << data.length();

    writing = true;
    emit isWritingChanged();

    bool succeed = false;
    QFileInfo info(filePath);
    if (QDir().mkpath(info.absolutePath())) {
        QFile file(info.filePath());
        if (file.exists())
            file.remove();
        if (file.open(QFile::WriteOnly))
            succeed = file.write(data) > 0;
        if (file.error() != QFile::NoError) {
            qDebug() << Q_FUNC_INFO << filePath << file.error();
            emit error(file.errorString());
        }
    }

    if (succeed) {
        QString thumbnail = info.baseName().replace('.', '-') + "-thumb.png";
        QStringList args = QStringList() << "-geometry" << "128x128" << info.fileName()+"[0]" << thumbnail;
        qDebug() << Q_FUNC_INFO << "/usr/bin/convert" << args;

        QProcess process;
        process.setWorkingDirectory(info.absolutePath());
        process.start("/usr/bin/convert", args);
        if (!process.waitForFinished())
            emit error(process.errorString());
        else
            emit written(info.filePath());
    }

    writing = false;
    emit isWritingChanged();
}