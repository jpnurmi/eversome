#include "manager.h"
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

Manager::Manager(QObject *parent) : QObject(parent)
{
    m_user = new UserStore(this);
    m_note = new NoteStore(this);

    connect(m_user, SIGNAL(loggedIn()), this, SLOT(onLoggedIn()), Qt::QueuedConnection);
    connect(m_user, SIGNAL(loggedOut()), this, SLOT(onLoggedOut()), Qt::QueuedConnection);

    connect(m_note, SIGNAL(synced(QVector<evernote::edam::Notebook>,
                                  QVector<evernote::edam::Resource>,
                                  QVector<evernote::edam::Note>,
                                  QVector<evernote::edam::Tag>)),
              this, SLOT(onSynced(QVector<evernote::edam::Notebook>,
                                  QVector<evernote::edam::Resource>,
                                  QVector<evernote::edam::Note>,
                                  QVector<evernote::edam::Tag>)), Qt::QueuedConnection);

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
    connect(m_database, SIGNAL(loaded(QList<NotebookItem*>,
                                      QList<ResourceItem*>,
                                      QList<NoteItem*>,
                                      QList<TagItem*>)),
                  this, SLOT(onLoaded(QList<NotebookItem*>,
                                      QList<ResourceItem*>,
                                      QList<NoteItem*>,
                                      QList<TagItem*>)), Qt::QueuedConnection);

    m_notebooks = new ItemModel(this);
    m_resources = new ItemModel(this);
    m_notes = new ItemModel(this);
    m_tags = new ItemModel(this);
}

Manager::~Manager()
{
}

Manager* Manager::instance()
{
    static Manager session;
    return &session;
}

UserStore* Manager::userStore() const
{
    return m_user;
}

NoteStore* Manager::noteStore() const
{
    return m_note;
}

ItemModel* Manager::notebookModel() const
{
    return m_notebooks;
}

ItemModel* Manager::resourceModel() const
{
    return m_resources;
}

ItemModel* Manager::noteModel() const
{
    return m_notes;
}

ItemModel* Manager::tagModel() const
{
    return m_tags;
}

void Manager::onLoggedIn()
{
    m_database->load(this);
    m_note->sync();
}

void Manager::onLoggedOut()
{
    m_note->cancel();
    Settings::reset();
    m_database->reset();

    m_notebooks->clear();
    m_resources->clear();
    m_notes->clear();
    m_tags->clear();
}

void Manager::onLoaded(const QList<NotebookItem*>& notebooks,
                       const QList<ResourceItem*>& resources,
                       const QList<NoteItem*>& notes,
                       const QList<TagItem*>& tags)
{
    m_notebooks->add(notebooks);
    m_resources->add(resources);
    m_notes->add(notes);
    m_tags->add(tags);

    setupNotes(m_notes->items<NoteItem*>());
}

void Manager::onSynced(const QVector<evernote::edam::Notebook>& notebooks,
                       const QVector<evernote::edam::Resource>& resources,
                       const QVector<evernote::edam::Note>& notes,
                       const QVector<evernote::edam::Tag>& tags)
{
    QList<NotebookItem*> notebookItems;
    foreach (const evernote::edam::Notebook& notebook, notebooks)
        notebookItems += new NotebookItem(notebook, this);
    m_notebooks->add(notebookItems);

    QList<ResourceItem*> resourceItems;
    foreach (const evernote::edam::Resource& resource, resources)
        resourceItems += new ResourceItem(resource, this);
    m_resources->add(resourceItems);

    QList<NoteItem*> noteItems;
    foreach (const evernote::edam::Note& note, notes)
        noteItems += new NoteItem(note, this);
    m_notes->add(noteItems);

    QList<TagItem*> tagItems;
    foreach (const evernote::edam::Tag& tag, tags)
        tagItems += new TagItem(tag, this);
    m_tags->add(tagItems);

    m_database->save(m_notebooks->items<NotebookItem*>(),
                     m_resources->items<ResourceItem*>(),
                     m_notes->items<NoteItem*>(),
                     m_tags->items<TagItem*>());

    setupNotes(m_notes->items<NoteItem*>());
}

void Manager::onNoteFetched(const evernote::edam::Note& note)
{
    NoteItem* item = m_notes->get<NoteItem*>(QString::fromStdString(note.guid));
    if (item)
        item->setContent(note.content);
}

void Manager::onResourceFetched(const evernote::edam::Resource& resource)
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

void Manager::setupNotes(const QList<NoteItem*>& notes)
{
    foreach (NoteItem* note, notes) {
        QString notebookGuid = QString::fromStdString(note->note().notebookGuid);
        NotebookItem* notebook = m_notebooks->get<NotebookItem*>(notebookGuid);
        if (notebook)
            notebook->notes()->add(QList<NoteItem*>() << note);
        else
            qCritical() << Q_FUNC_INFO << "MISSING NOTEBOOK:" << notebookGuid;

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
