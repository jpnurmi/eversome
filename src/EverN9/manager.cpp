#include "manager.h"
#include "database.h"
#include "settings.h"
#include "userstore.h"
#include "notestore.h"
#include "notebookitem.h"
#include "resourceitem.h"
#include "searchitem.h"
#include "itemmodel.h"
#include "noteitem.h"
#include "tagitem.h"
#include <QVector>
#include <QDebug>

Manager::Manager(QObject *parent) : QObject(parent)
{
    m_user = new UserStore(this);
    m_note = new NoteStore(this);

    connect(m_user, SIGNAL(loggedIn()), this, SLOT(onLoggedIn()), Qt::QueuedConnection);
    connect(m_user, SIGNAL(loggedOut()), this, SLOT(onLoggedOut()), Qt::QueuedConnection);

    connect(m_note, SIGNAL(synced(QVector<evernote::edam::Notebook>,
                                  QVector<evernote::edam::Resource>,
                                  QVector<evernote::edam::SavedSearch>,
                                  QVector<evernote::edam::Note>,
                                  QVector<evernote::edam::Tag>)),
              this, SLOT(onSynced(QVector<evernote::edam::Notebook>,
                                  QVector<evernote::edam::Resource>,
                                  QVector<evernote::edam::SavedSearch>,
                                  QVector<evernote::edam::Note>,
                                  QVector<evernote::edam::Tag>)), Qt::QueuedConnection);

    connect(m_note, SIGNAL(resourceFetched(evernote::edam::Resource)),
              this, SLOT(onResourceFetched(evernote::edam::Resource)), Qt::QueuedConnection);
    connect(m_note, SIGNAL(noteFetched(evernote::edam::Note)),
              this, SLOT(onNoteFetched(evernote::edam::Note)), Qt::QueuedConnection);
    connect(m_note, SIGNAL(searched(evernote::edam::SavedSearch,QVector<evernote::edam::Note>)),
              this, SLOT(onSearched(evernote::edam::SavedSearch,QVector<evernote::edam::Note>)), Qt::QueuedConnection);

    qRegisterMetaType<TagItem*>();
    qRegisterMetaType<NoteItem*>();
    qRegisterMetaType<SearchItem*>();
    qRegisterMetaType<ResourceItem*>();
    qRegisterMetaType<NotebookItem*>();
    qRegisterMetaType<QList<TagItem*> >();
    qRegisterMetaType<QList<NoteItem*> >();
    qRegisterMetaType<QList<SearchItem*> >();
    qRegisterMetaType<QList<ResourceItem*> >();
    qRegisterMetaType<QList<NotebookItem*> >();

    m_database = new Database(this);
    connect(m_database, SIGNAL(loaded(QList<NotebookItem*>,
                                      QList<ResourceItem*>,
                                      QList<SearchItem*>,
                                      QList<NoteItem*>,
                                      QList<TagItem*>)),
                  this, SLOT(onLoaded(QList<NotebookItem*>,
                                      QList<ResourceItem*>,
                                      QList<SearchItem*>,
                                      QList<NoteItem*>,
                                      QList<TagItem*>)), Qt::QueuedConnection);

    m_notebooks = new ItemModel(this);
    m_resources = new ItemModel(this);
    m_searches = new ItemModel(this);
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

Database* Manager::database() const
{
    return m_database;
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

ItemModel* Manager::searchModel() const
{
    return m_searches;
}

ItemModel* Manager::noteModel() const
{
    return m_notes;
}

ItemModel* Manager::tagModel() const
{
    return m_tags;
}

QString Manager::errorString(int code)
{
    switch (code) {
        case evernote::edam::BAD_DATA_FORMAT:       return tr("The format of the request data was incorrect");
        case evernote::edam::PERMISSION_DENIED:     return tr("Not permitted to perform action");
        case evernote::edam::INTERNAL_ERROR:        return tr("Unexpected problem with the service");
        case evernote::edam::DATA_REQUIRED:         return tr("A required parameter/field was absent");
        case evernote::edam::LIMIT_REACHED:         return tr("Operation denied due to data model limit");
        case evernote::edam::QUOTA_REACHED:         return tr("Operation denied due to user storage limit");
        case evernote::edam::INVALID_AUTH:          return tr("Incorrect username and/or password");
        case evernote::edam::AUTH_EXPIRED:          return tr("Authentication token expired");
        case evernote::edam::DATA_CONFLICT:         return tr("Change denied due to data model conflict");
        case evernote::edam::ENML_VALIDATION:       return tr("Content of submitted note was malformed");
        case evernote::edam::SHARD_UNAVAILABLE:     return tr("Service shard with account data is temporarily down");
        case evernote::edam::LEN_TOO_SHORT:         return tr("Operation denied due to data model limit (too short)");
        case evernote::edam::LEN_TOO_LONG:          return tr("Operation denied due to data model limit (too long)");
        case evernote::edam::TOO_FEW:               return tr("Operation denied due to data model limit (too few)");
        case evernote::edam::TOO_MANY:              return tr("Operation denied due to data model limit (too many)");
        case evernote::edam::UNSUPPORTED_OPERATION: return tr("Operation denied because it is currently unsupported");
        case -1:                                    return tr("Unknown operation");
        case evernote::edam::UNKNOWN:
        default:                                    return tr("Unknown error occurred");
    }
}

void Manager::onLoggedIn()
{
    m_database->load(this);
    m_note->sync();
}

void Manager::onLoggedOut()
{
    m_note->cancel();
    m_database->reset();

    m_notebooks->clear();
    m_resources->clear();
    m_searches->clear();
    m_notes->clear();
    m_tags->clear();
}

void Manager::onLoaded(const QList<NotebookItem*>& notebooks,
                       const QList<ResourceItem*>& resources,
                       const QList<SearchItem*>& searches,
                       const QList<NoteItem*>& notes,
                       const QList<TagItem*>& tags)
{
    m_notebooks->add(notebooks);
    m_resources->add(resources);
    m_searches->add(searches);
    m_notes->add(notes);
    m_tags->add(tags);

    setupNotes(m_notes->items<NoteItem*>());
}

void Manager::onSynced(const QVector<evernote::edam::Notebook>& notebooks,
                       const QVector<evernote::edam::Resource>& resources,
                       const QVector<evernote::edam::SavedSearch>& searches,
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

    QList<SearchItem*> searchItems;
    foreach (const evernote::edam::SavedSearch& search, searches)
        searchItems += new SearchItem(search, this);
    m_searches->add(searchItems);

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
                     m_searches->items<SearchItem*>(),
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

void Manager::onSearched(const evernote::edam::SavedSearch& search, const QVector<evernote::edam::Note>& notes)
{
    SearchItem* searchItem = m_searches->get<SearchItem*>(QString::fromStdString(search.guid));
    if (searchItem) {
        QList<NoteItem*> noteItems;
        foreach (const evernote::edam::Note& note, notes) {
            NoteItem* noteItem = m_notes->get<NoteItem*>(QString::fromStdString(note.guid));
            if (noteItem)
                noteItems += noteItem;
        }
        searchItem->notes()->add(noteItems);
    }
}

void Manager::onResourceFetched(const evernote::edam::Resource& resource)
{
    ResourceItem* item = m_resources->get<ResourceItem*>(QString::fromStdString(resource.guid));
    if (item)
        item->setData(resource.data);
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

        for (uint i = 0; i < note->note().resources.size(); ++i) {
            QString resourceGuid = QString::fromStdString(note->note().resources.at(i).guid);
            ResourceItem* resource = m_resources->get<ResourceItem*>(resourceGuid);
            if (resource)
                note->resources()->add(QList<ResourceItem*>() << resource);
            else
                qCritical() << Q_FUNC_INFO << "MISSING RESOURCE:" << resourceGuid;
        }

        for (uint i = 0; i < note->note().tagGuids.size(); ++i) {
            QString tagGuid = QString::fromStdString(note->note().tagGuids.at(i));
            TagItem* tag = m_tags->get<TagItem*>(tagGuid);
            if (!tag) {
                qCritical() << Q_FUNC_INFO << "MISSING TAG:" << tagGuid;
                continue;
            }
            note->tags()->add(QList<TagItem*>() << tag);
            tag->notes()->add(QList<NoteItem*>() << note);
        }
    }
}
