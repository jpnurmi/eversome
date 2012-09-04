#include "session.h"
#include "database.h"
#include "settings.h"
#include "userstore.h"
#include "notestore.h"
#include "notebookitem.h"
#include "resourceitem.h"
#include "itemmodel.h"
#include "noteitem.h"
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

    qRegisterMetaType<TagItem*>();
    qRegisterMetaType<NoteItem*>();
    qRegisterMetaType<ResourceItem*>();
    qRegisterMetaType<NotebookItem*>();
    qRegisterMetaType<QList<TagItem*> >();
    qRegisterMetaType<QList<NoteItem*> >();
    qRegisterMetaType<QList<ResourceItem*> >();
    qRegisterMetaType<QList<NotebookItem*> >();

    m_database = new Database(this);
    connect(m_database, SIGNAL(notebooksLoaded(QList<NotebookItem*>)),
                  this, SLOT(onNotebooksLoaded(QList<NotebookItem*>)), Qt::QueuedConnection);
    connect(m_database, SIGNAL(resourcesLoaded(QList<ResourceItem*>)),
                  this, SLOT(onResourcesLoaded(QList<ResourceItem*>)), Qt::QueuedConnection);
    connect(m_database, SIGNAL(notesLoaded(QList<NoteItem*>)),
                  this, SLOT(onNotesLoaded(QList<NoteItem*>)), Qt::QueuedConnection);
    connect(m_database, SIGNAL(tagsLoaded(QList<TagItem*>)),
                  this, SLOT(onTagsLoaded(QList<TagItem*>)), Qt::QueuedConnection);

    m_notebooks = new ItemModel(this);
    m_resources = new ItemModel(this);
    m_notes = new ItemModel(this);
    m_tags = new ItemModel(this);
}

Session::~Session()
{
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

ItemModel* Session::notebookModel() const
{
    return m_notebooks;
}

ItemModel* Session::resourceModel() const
{
    return m_resources;
}

ItemModel* Session::noteModel() const
{
    return m_notes;
}

ItemModel* Session::tagModel() const
{
    return m_tags;
}

void Session::onLoggedIn()
{
    m_database->load(this);
    m_note->sync();
}

void Session::onLoggedOut()
{
    m_note->cancel();
    Settings::reset();
    m_database->reset();

    m_notebooks->clear();
    m_resources->clear();
    m_notes->clear();
    m_tags->clear();
}

void Session::onNotebooksLoaded(const QList<NotebookItem*>& notebooks)
{
    m_notebooks->add(notebooks);
    setupNotebooks(m_notes->items<NoteItem*>());
}

void Session::onResourcesLoaded(const QList<ResourceItem*>& resources)
{
    m_resources->add(resources);
}

void Session::onNotesLoaded(const QList<NoteItem*>& notes)
{
    m_notes->add(notes);
}

void Session::onTagsLoaded(const QList<TagItem*>& tags)
{
    m_tags->add(tags);
    setupTags(m_notes->items<NoteItem*>());
}

void Session::onNotebooksSynced(const QVector<evernote::edam::Notebook>& notebooks)
{
    QList<NotebookItem*> items;
    foreach (const evernote::edam::Notebook& notebook, notebooks)
        items += new NotebookItem(notebook, this);
    m_notebooks->add(items);
    m_database->saveNotebooks(m_notebooks->items<NotebookItem*>());
    setupNotebooks(m_notes->items<NoteItem*>());
}

void Session::onResourcesSynced(const QVector<evernote::edam::Resource>& resources)
{
    QList<ResourceItem*> items;
    foreach (const evernote::edam::Resource& resource, resources)
        items += new ResourceItem(resource, this);
    m_resources->add(items);
    m_database->saveResources(m_resources->items<ResourceItem*>());
}

void Session::onNotesSynced(const QVector<evernote::edam::Note>& notes)
{
    QList<NoteItem*> items;
    foreach (const evernote::edam::Note& note, notes)
        items += new NoteItem(note, this);
    m_notes->add(items);
    m_database->saveNotes(m_notes->items<NoteItem*>());
}

void Session::onTagsSynced(const QVector<evernote::edam::Tag>& tags)
{
    QList<TagItem*> items;
    foreach (const evernote::edam::Tag& tag, tags)
        items += new TagItem(tag, this);
    m_tags->add(items);
    m_database->saveTags(m_tags->items<TagItem*>());
    setupTags(m_notes->items<NoteItem*>());
}

void Session::onNoteFetched(const evernote::edam::Note& note)
{
    NoteItem* item = m_notes->get<NoteItem*>(QString::fromStdString(note.guid));
    if (item)
        item->setContent(note.content);
}

void Session::onResourceFetched(const evernote::edam::Resource& resource)
{
    ResourceItem* item = m_resources->get<ResourceItem*>(QString::fromStdString(resource.guid));
    if (item) {
        QFileInfo info(item->filePath());
        if (QDir().mkpath(info.absolutePath())) {
            QFile file(item->filePath());
            if (file.open(QFile::WriteOnly))
                file.write(resource.data.body.c_str(), resource.data.size);
        }
    }
}

void Session::setupNotebooks(const QList<NoteItem*>& notes)
{
    foreach (NoteItem* note, notes) {
        QString notebookGuid = QString::fromStdString(note->note().notebookGuid);
        NotebookItem* notebook = m_notebooks->get<NotebookItem*>(notebookGuid);
        if (notebook)
            notebook->notes()->add(QList<NoteItem*>() << note);
        else
            qCritical() << Q_FUNC_INFO << "MISSING NOTEBOOK:" << notebookGuid;
    }
}

void Session::setupTags(const QList<NoteItem*>& notes)
{
    foreach (NoteItem* note, notes) {
        for (uint i = 0; i < note->note().tagGuids.size(); ++i) {
            QString tagGuid = QString::fromStdString(note->note().tagGuids.at(i));
            TagItem* tag = m_tags->get<TagItem*>(tagGuid);
            if (tag)
                tag->notes()->add(QList<NoteItem*>() << note);
            else
                qCritical() << Q_FUNC_INFO << "MISSING TAG:" << tagGuid;
        }
    }
}
