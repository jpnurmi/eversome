#include "session.h"
#include "database.h"
#include "synchronizer.h"
#include "authenticator.h"
#include "notebookmodel.h"
#include "notebookitem.h"
#include "notemodel.h"
#include "noteitem.h"
#include "tagmodel.h"
#include "tagitem.h"
#include <QVector>

Session::Session(QObject *parent) : QObject(parent)
{
    m_auth = new Authenticator(this);
    m_sync = new Synchronizer(this);

    connect(m_auth, SIGNAL(succeed()), m_sync, SLOT(sync()), Qt::QueuedConnection);

    connect(m_sync, SIGNAL(notebooksSynced(QVector<evernote::edam::Notebook>)),
              this, SLOT(syncNotebooks(QVector<evernote::edam::Notebook>)), Qt::QueuedConnection);
    connect(m_sync, SIGNAL(resourcesSynced(QVector<evernote::edam::Resource>)),
              this, SLOT(syncResources(QVector<evernote::edam::Resource>)), Qt::QueuedConnection);
    connect(m_sync, SIGNAL(notesSynced(QVector<evernote::edam::Note>)),
              this, SLOT(syncNotes(QVector<evernote::edam::Note>)), Qt::QueuedConnection);
    connect(m_sync, SIGNAL(tagsSynced(QVector<evernote::edam::Tag>)),
              this, SLOT(syncTags(QVector<evernote::edam::Tag>)), Qt::QueuedConnection);

    Database::initialize();
    TagModel::instance()->add(Database::loadTags(this));
    NotebookModel::instance()->add(Database::loadNotebooks(this));
}

Session::~Session()
{
    Database::uninitialize();
}

Authenticator* Session::authenticator() const
{
    return m_auth;
}

Synchronizer* Session::synchronizer() const
{
    return m_sync;
}

void Session::syncNotebooks(const QVector<evernote::edam::Notebook>& notebooks)
{
    QList<NotebookItem*> items;
    foreach (const evernote::edam::Notebook& notebook, notebooks)
        items += new NotebookItem(notebook, this);
    NotebookModel::instance()->add(items);
    Database::saveNotebooks(NotebookModel::instance()->notebooks());
}

void Session::syncResources(const QVector<evernote::edam::Resource>& resources)
{
    // TODO
}

void Session::syncNotes(const QVector<evernote::edam::Note>& notes)
{
    foreach (const evernote::edam::Note& note, notes) {
        QString guid = QString::fromStdString(note.notebookGuid);
        NotebookItem* notebook = NotebookModel::instance()->get(guid);
        NoteItem* item = new NoteItem(note, notebook);
        notebook->notes()->addNote(item);
    }
}

void Session::syncTags(const QVector<evernote::edam::Tag>& tags)
{
    QList<TagItem*> items;
    foreach (const evernote::edam::Tag& tag, tags)
        items += new TagItem(tag, this);
    TagModel::instance()->add(items);
    Database::saveTags(TagModel::instance()->tags());
}
