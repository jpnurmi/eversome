#include "session.h"
#include "database.h"
#include "synchronizer.h"
#include "authenticator.h"
#include "notebookmodel.h"
#include "notebookitem.h"
#include "resourceitem.h"
#include "notemodel.h"
#include "noteitem.h"
#include "tagmodel.h"
#include "tagitem.h"
#include <QVector>
#include <QDebug>

static void setupNotes(const QList<NoteItem*>& notes)
{
    foreach (NoteItem* note, notes) {
        QString notebookGuid = QString::fromStdString(note->note().notebookGuid);
        NotebookItem* notebook = NotebookModel::instance()->get(notebookGuid);
        if (notebook)
            notebook->notes()->add(note);
        else
            qCritical() << Q_FUNC_INFO << "MISSING NOTEBOOK:" << notebookGuid;

        for (uint i = 0; i < note->note().tagGuids.size(); ++i) {
            QString tagGuid = QString::fromStdString(note->note().tagGuids.at(i));
            TagItem* tag = TagModel::instance()->get(tagGuid);
            if (tag)
                tag->notes()->add(note);
            else
                qCritical() << Q_FUNC_INFO << "MISSING TAG:" << tagGuid;
        }
    }
}

Session::Session(QObject *parent) : QObject(parent)
{
    m_auth = new Authenticator(this);
    m_sync = new Synchronizer(this);

    connect(m_auth, SIGNAL(succeed()), m_sync, SLOT(sync()), Qt::QueuedConnection);

    connect(m_sync, SIGNAL(notebooksSynced(QVector<evernote::edam::Notebook>)),
              this, SLOT(onNotebooksSynced(QVector<evernote::edam::Notebook>)), Qt::QueuedConnection);
    connect(m_sync, SIGNAL(resourcesSynced(QVector<evernote::edam::Resource>)),
              this, SLOT(onResourcesSynced(QVector<evernote::edam::Resource>)), Qt::QueuedConnection);
    connect(m_sync, SIGNAL(notesSynced(QVector<evernote::edam::Note>)),
              this, SLOT(onNotesSynced(QVector<evernote::edam::Note>)), Qt::QueuedConnection);
    connect(m_sync, SIGNAL(tagsSynced(QVector<evernote::edam::Tag>)),
              this, SLOT(onTagsSynced(QVector<evernote::edam::Tag>)), Qt::QueuedConnection);
    connect(m_sync, SIGNAL(resourceFetched(ResourceItem*)),
              this, SLOT(onResourceFetched(ResourceItem*)));
    connect(m_sync, SIGNAL(noteFetched(NoteItem*)),
              this, SLOT(onNoteFetched(NoteItem*)));

    Database::initialize();
    TagModel::instance()->add(Database::loadTags(this));
    NotebookModel::instance()->add(Database::loadNotebooks(this));
    setupNotes(Database::loadNotes(this));
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

void Session::onNotebooksSynced(const QVector<evernote::edam::Notebook>& notebooks)
{
    QList<NotebookItem*> items;
    foreach (const evernote::edam::Notebook& notebook, notebooks)
        items += new NotebookItem(notebook, this);
    NotebookModel::instance()->add(items);
    Database::saveNotebooks(NotebookModel::instance()->notebooks());
}

void Session::onResourcesSynced(const QVector<evernote::edam::Resource>& resources)
{
    // TODO
}

void Session::onNotesSynced(const QVector<evernote::edam::Note>& notes)
{
    QList<NoteItem*> items;
    foreach (const evernote::edam::Note& note, notes)
        items += new NoteItem(note, this);
    Database::saveNotes(NoteModel::allNotes.values());
    setupNotes(items);
}

void Session::onTagsSynced(const QVector<evernote::edam::Tag>& tags)
{
    QList<TagItem*> items;
    foreach (const evernote::edam::Tag& tag, tags)
        items += new TagItem(tag, this);
    TagModel::instance()->add(items);
    Database::saveTags(TagModel::instance()->tags());
}

void Session::onResourceFetched(ResourceItem* resource)
{
    // TODO
}

void Session::onNoteFetched(NoteItem* note)
{
    Database::saveNotes(QList<NoteItem*>() << note);
}
