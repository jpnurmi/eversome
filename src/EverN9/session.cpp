#include "session.h"
#include "database.h"
#include "synchronizer.h"
#include "authenticator.h"
#include "notebookmodel.h"
#include "resourcemodel.h"
#include "notebookitem.h"
#include "resourceitem.h"
#include "notemodel.h"
#include "noteitem.h"
#include "tagmodel.h"
#include "tagitem.h"
#include <QFileInfo>
#include <QVector>
#include <QDebug>
#include <QFile>
#include <QDir>

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
    connect(m_sync, SIGNAL(resourceFetched(evernote::edam::Resource)),
              this, SLOT(onResourceFetched(evernote::edam::Resource)), Qt::QueuedConnection);
    connect(m_sync, SIGNAL(noteFetched(evernote::edam::Note)),
              this, SLOT(onNoteFetched(evernote::edam::Note)), Qt::QueuedConnection);

    Database::initialize();
    TagModel::instance()->add(Database::loadTags(this));
    ResourceModel::instance()->add(Database::loadResources(this));
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
    QList<ResourceItem*> items;
    foreach (const evernote::edam::Resource& resource, resources)
        items += new ResourceItem(resource, this);
    ResourceModel::instance()->add(items);
    Database::saveResources(ResourceModel::instance()->resources());
}

void Session::onNotesSynced(const QVector<evernote::edam::Note>& notes)
{
    QList<NoteItem*> items;
    foreach (const evernote::edam::Note& note, notes)
        items += new NoteItem(note, this);
    Database::saveNotes(NoteItem::allNotes.values());
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

void Session::onNoteFetched(const evernote::edam::Note& note)
{
    NoteItem* item = NoteItem::get(QString::fromStdString(note.guid));
    if (item) {
        item->setContent(note.content);
    }
}

void Session::onResourceFetched(const evernote::edam::Resource& resource)
{
    ResourceItem* item = ResourceModel::instance()->get(QString::fromStdString(resource.guid));
    if (item) {
        QFileInfo info(item->filePath());
        if (QDir().mkpath(info.absolutePath())) {
            QFile file(item->filePath());
            if (file.open(QFile::WriteOnly))
                file.write(resource.data.body.c_str(), resource.data.size);
        }
    }
}
