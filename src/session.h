#ifndef SESSION_H
#define SESSION_H

#include <QObject>
#include "edam/Types_types.h"

class Authenticator;
class Synchronizer;

class Session : public QObject
{
    Q_OBJECT

public:
    explicit Session(QObject* parent = 0);
    virtual ~Session();

    Authenticator* authenticator() const;
    Synchronizer* synchronizer() const;

private slots:
    void syncNotebooks(const QVector<evernote::edam::Notebook>& notebooks);
    void syncResources(const QVector<evernote::edam::Resource>& resources);
    void syncNotes(const QVector<evernote::edam::Note>& notes);
    void syncTags(const QVector<evernote::edam::Tag>& tags);

private:
    Authenticator* m_auth;
    Synchronizer* m_sync;
};

#endif // SESSION_H
