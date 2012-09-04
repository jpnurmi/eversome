#include "session.h"
#include "database.h"
#include "settings.h"
#include "userstore.h"
#include "notestore.h"
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

Session::Session(QObject *parent) : QObject(parent)
{
    m_user = new UserStore(this);
    m_note = new NoteStore(this);

    connect(m_user, SIGNAL(loggedIn()), this, SLOT(onLoggedIn()), Qt::QueuedConnection);
    connect(m_user, SIGNAL(loggedOut()), this, SLOT(onLoggedOut()), Qt::QueuedConnection);

    connect(m_note, SIGNAL(notebooksSynced(QVector<evernote::edam::Notebook>)),
              this, SLOT(onNotebooksSynced(QVector<evernote::edam::Notebook>)), Qt::QueuedConnection);
    connect(m_note, SIGNAL(resourcesSynced(QVector<evernote::edam::Resource>)),
              this, SLOT(onResourcesSynced(QVector<evernote::edam::Resource>)), Qt::QueuedConnection);
    connect(m_note, SIGNAL(notesSynced(QVector<evernote::edam::Note>)),
              this, SLOT(onNotesSynced(QVector<evernote::edam::Note>)), Qt::QueuedConnection);
    connect(m_note, SIGNAL(tagsSynced(QVector<evernote::edam::Tag>)),
              this, SLOT(onTagsSynced(QVector<evernote::edam::Tag>)), Qt::QueuedConnection);
    connect(m_note, SIGNAL(resourceFetched(evernote::edam::Resource)),
              this, SLOT(onResourceFetched(evernote::edam::Resource)), Qt::QueuedConnection);
    connect(m_note, SIGNAL(noteFetched(evernote::edam::Note)),
              this, SLOT(onNoteFetched(evernote::edam::Note)), Qt::QueuedConnection);

    m_notebooks = new NotebookModel(this);
    m_resources = new ResourceModel(this);
    m_tags = new TagModel(this);

    Database::initialize();
}

Session::~Session()
{
    Database::uninitialize();
}

Session* Session::instance()
{
    static Session session;
    return &session;
}

UserStore* Session::userStore() const
{
    return m_user;
}

NoteStore* Session::noteStore() const
{
    return m_note;
}

NotebookModel* Session::notebookModel() const
{
    return m_notebooks;
}

ResourceModel* Session::resourceModel() const
{
    return m_resources;
}

TagModel* Session::tagModel() const
{
    return m_tags;
}

void Session::onLoggedIn()
{
    m_tags->add(Database::loadTags(this));
    m_resources->add(Database::loadResources(this));
    m_notebooks->add(Database::loadNotebooks(this));
    setupNotes(Database::loadNotes(this));

    m_note->sync();
}

void Session::onLoggedOut()
{
    m_note->cancel();
    Settings::reset();
    Database::reset();

    m_notebooks->clear();
    m_resources->clear();
    m_tags->clear();
}

void Session::onNotebooksSynced(const QVector<evernote::edam::Notebook>& notebooks)
{
    QList<NotebookItem*> items;
    foreach (const evernote::edam::Notebook& notebook, notebooks)
        items += new NotebookItem(notebook, this);
    m_notebooks->add(items);
    Database::saveNotebooks(m_notebooks->notebooks());
}

void Session::onResourcesSynced(const QVector<evernote::edam::Resource>& resources)
{
    QList<ResourceItem*> items;
    foreach (const evernote::edam::Resource& resource, resources)
        items += new ResourceItem(resource, this);
    m_resources->add(items);
    Database::saveResources(m_resources->resources());
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
    m_tags->add(items);
    Database::saveTags(m_tags->tags());
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
    ResourceItem* item = m_resources->get(QString::fromStdString(resource.guid));
    if (item) {
        QFileInfo info(item->filePath());
        if (QDir().mkpath(info.absolutePath())) {
            QFile file(item->filePath());
            if (file.open(QFile::WriteOnly))
                file.write(resource.data.body.c_str(), resource.data.size);
        }
    }
}

void Session::setupNotes(const QList<NoteItem*>& notes)
{
    foreach (NoteItem* note, notes) {
        QString notebookGuid = QString::fromStdString(note->note().notebookGuid);
        NotebookItem* notebook = m_notebooks->get(notebookGuid);
        if (notebook)
            notebook->notes()->add(note);
        else
            qCritical() << Q_FUNC_INFO << "MISSING NOTEBOOK:" << notebookGuid;

        for (uint i = 0; i < note->note().tagGuids.size(); ++i) {
            QString tagGuid = QString::fromStdString(note->note().tagGuids.at(i));
            TagItem* tag = m_tags->get(tagGuid);
            if (tag)
                tag->notes()->add(note);
            else
                qCritical() << Q_FUNC_INFO << "MISSING TAG:" << tagGuid;
        }
    }
}
