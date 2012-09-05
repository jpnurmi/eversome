#ifndef RESOURCEWRITER_H
#define RESOURCEWRITER_H

#include <QSet>
#include <QObject>
#include <QProcess>

class ResourceWriter : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool isWriting READ isWriting NOTIFY isWritingChanged)

public:
    explicit ResourceWriter(QObject* parent = 0);
    virtual ~ResourceWriter();

    bool isWriting() const;

    void write(const QString& filePath, const QByteArray& data);

signals:
    void isWritingChanged();
    void error(const QString& error);
    void written(const QString& filePath);

private slots:
    void writeImpl(const QString& filePath, const QByteArray& data);

private:
    QSet<QString> m_files;
};

#endif // RESOURCEWRITER_H
