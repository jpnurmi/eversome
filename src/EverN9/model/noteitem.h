#ifndef NOTEITEM_H
#define NOTEITEM_H

#include <QObject>
#include <QDateTime>
#include <QMetaType>
#include "edam/Types_types.h"
#include "itemmodel.h"

class NoteItem : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString guid READ guid CONSTANT)
    Q_PROPERTY(QString title READ title CONSTANT)
    Q_PROPERTY(QString content READ content NOTIFY contentChanged)
    Q_PROPERTY(QDateTime created READ created CONSTANT)
    Q_PROPERTY(QDateTime updated READ updated CONSTANT)
    Q_PROPERTY(QDateTime deleted READ deleted CONSTANT)
    Q_PROPERTY(bool isActive READ isActive CONSTANT)
    Q_PROPERTY(bool isEmpty READ isEmpty NOTIFY isEmptyChanged)
    Q_PROPERTY(ItemModel* tags READ tags CONSTANT)
    Q_PROPERTY(ItemModel* resources READ resources CONSTANT)

public:
    explicit NoteItem(evernote::edam::Note note = evernote::edam::Note(), QObject* parent = 0);
    virtual ~NoteItem();

    Q_INVOKABLE evernote::edam::Note note() const;

    QString guid() const;
    QString title() const;
    QString content() const;
    QDateTime created() const;
    QDateTime updated() const;
    QDateTime deleted() const;
    bool isActive() const;
    bool isEmpty() const;
    ItemModel* tags() const;
    ItemModel* resources() const;

    void setContent(const std::string& content);

signals:
    void isEmptyChanged();
    void contentChanged();

private:
    ItemModel* m_tags;
    ItemModel* m_resources;
    evernote::edam::Note m_note;
};

Q_DECLARE_METATYPE(NoteItem*)
Q_DECLARE_METATYPE(QList<NoteItem*>)

#endif // NOTEITEM_H
