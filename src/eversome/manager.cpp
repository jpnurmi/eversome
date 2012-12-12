/*
* Copyright (C) 2012 J-P Nurmi <jpnurmi@gmail.com>
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*/
#include "manager.h"
#include "session.h"
#include "database.h"
#include "syncpool.h"
#include "filesystem.h"
#include "networkpool.h"
#include "notebookitem.h"
#include "resourceitem.h"
#include "searchitem.h"
#include "itemmodel.h"
#include "noteitem.h"
#include "tagitem.h"
#include <QVector>
#include <QTimer>
#include <QDebug>

Manager::Manager(Session* session) : QObject(session)
{
    connect(session, SIGNAL(error(QString)), this, SIGNAL(error(QString)));

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

    m_networkpool = new NetworkPool(session);
    connect(m_networkpool, SIGNAL(error(QString)), this, SIGNAL(error(QString)));

    connect(m_networkpool, SIGNAL(noteCreated(evernote::edam::Note)),
            this, SLOT(onNoteCreated(evernote::edam::Note)), Qt::QueuedConnection);
    connect(m_networkpool, SIGNAL(noteFetched(evernote::edam::Note)),
            this, SLOT(onNoteFetched(evernote::edam::Note)), Qt::QueuedConnection);
    connect(m_networkpool, SIGNAL(noteMoved(evernote::edam::Note)),
            this, SLOT(onNoteMoved(evernote::edam::Note)), Qt::QueuedConnection);
    connect(m_networkpool, SIGNAL(noteRenamed(evernote::edam::Note)),
            this, SLOT(onNoteRenamed(evernote::edam::Note)), Qt::QueuedConnection);
    connect(m_networkpool, SIGNAL(noteTrashed(evernote::edam::Note)),
            this, SLOT(onNoteTrashed(evernote::edam::Note)), Qt::QueuedConnection);

    connect(m_networkpool, SIGNAL(searchCreated(evernote::edam::SavedSearch)),
            this, SLOT(onSearchCreated(evernote::edam::SavedSearch)), Qt::QueuedConnection);
    connect(m_networkpool, SIGNAL(searchFetched(evernote::edam::SavedSearch)),
            this, SLOT(onSearchFetched(evernote::edam::SavedSearch)), Qt::QueuedConnection);
    connect(m_networkpool, SIGNAL(searched(evernote::edam::SavedSearch,QVector<evernote::edam::NoteMetadata>)),
            this, SLOT(onSearched(evernote::edam::SavedSearch,QVector<evernote::edam::NoteMetadata>)), Qt::QueuedConnection);
    connect(m_networkpool, SIGNAL(searchRenamed(evernote::edam::SavedSearch)),
            this, SLOT(onSearchRenamed(evernote::edam::SavedSearch)), Qt::QueuedConnection);

    connect(m_networkpool, SIGNAL(notebookCreated(evernote::edam::Notebook)),
            this, SLOT(onNotebookCreated(evernote::edam::Notebook)), Qt::QueuedConnection);
    connect(m_networkpool, SIGNAL(notebookFetched(evernote::edam::Notebook)),
            this, SLOT(onNotebookFetched(evernote::edam::Notebook)), Qt::QueuedConnection);
    connect(m_networkpool, SIGNAL(notebookRenamed(evernote::edam::Notebook)),
            this, SLOT(onNotebookRenamed(evernote::edam::Notebook)), Qt::QueuedConnection);

    connect(m_networkpool, SIGNAL(tagCreated(evernote::edam::Tag)),
            this, SLOT(onTagCreated(evernote::edam::Tag)), Qt::QueuedConnection);
    connect(m_networkpool, SIGNAL(tagFetched(evernote::edam::Tag)),
            this, SLOT(onTagFetched(evernote::edam::Tag)), Qt::QueuedConnection);
    connect(m_networkpool, SIGNAL(tagRenamed(evernote::edam::Tag)),
            this, SLOT(onTagRenamed(evernote::edam::Tag)), Qt::QueuedConnection);

    connect(m_networkpool, SIGNAL(resourceFetched(evernote::edam::Resource)),
            this, SLOT(onResourceFetched(evernote::edam::Resource)), Qt::QueuedConnection);
    connect(m_networkpool, SIGNAL(thumbnailFetched(const QString& guid, const QByteArray& data)),
            this, SLOT(onThumbnailFetched(const QString& guid, const QByteArray& data)), Qt::QueuedConnection);

    m_syncpool = new SyncPool(session);
    connect(m_syncpool, SIGNAL(error(QString)), this, SIGNAL(error(QString)));
    connect(m_syncpool, SIGNAL(synced(QVector<evernote::edam::Notebook>,
                                       QVector<evernote::edam::Resource>,
                                       QVector<evernote::edam::SavedSearch>,
                                       QVector<evernote::edam::Note>,
                                       QVector<evernote::edam::Tag>)),
                   this, SLOT(onSynced(QVector<evernote::edam::Notebook>,
                                       QVector<evernote::edam::Resource>,
                                       QVector<evernote::edam::SavedSearch>,
                                       QVector<evernote::edam::Note>,
                                       QVector<evernote::edam::Tag>)), Qt::QueuedConnection);
    connect(m_syncpool, SIGNAL(expunged(QVector<std::string>,QVector<std::string>,
                                         QVector<std::string>,QVector<std::string>)),
                   this, SLOT(onExpunged(QVector<std::string>,QVector<std::string>,
                                         QVector<std::string>,QVector<std::string>)), Qt::QueuedConnection);

    m_files = new FileSystem(this);
    connect(m_files, SIGNAL(error(QString)), this, SIGNAL(error(QString)));
    connect(m_files, SIGNAL(wrote(QString,QString)), SLOT(onFileWritten(QString,QString)), Qt::QueuedConnection);

    m_database = new Database(this);
    connect(m_database, SIGNAL(error(QString)), this, SIGNAL(error(QString)));
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
    m_database->open();
    m_database->load(this);

    connect(m_syncpool, SIGNAL(activityChanged()), this, SLOT(onActivityChanged()));
    connect(m_files, SIGNAL(activityChanged()), this, SLOT(onActivityChanged()));
    connect(m_database, SIGNAL(activityChanged()), this, SLOT(onActivityChanged()));
    connect(m_networkpool, SIGNAL(activityChanged()), this, SLOT(onActivityChanged()));

    m_itemmodels[Notebook] = new ItemModel(this);
    m_itemmodels[Resource] = new ItemModel(this);
    m_itemmodels[Search] = new ItemModel(this);
    m_itemmodels[Note] = new ItemModel(this);
    m_itemmodels[Tag] = new ItemModel(this);

    m_itemmodels.value(Notebook)->setSortProperty("name");
    m_itemmodels.value(Search)->setSortProperty("name");
    m_itemmodels.value(Note)->setSortProperty("title");
    m_itemmodels.value(Tag)->setSortProperty("name");
}

Manager::~Manager()
{
    m_syncpool->waitForDone();
    m_files->waitForDone();
    m_database->close();
    m_database->waitForDone();
    m_networkpool->waitForDone();
    foreach (ItemModel* model, m_itemmodels)
        model->clear();
}

bool Manager::isBusy() const
{
    return m_syncpool->activeThreadCount() || m_files->activeThreadCount() ||
           m_database->activeThreadCount() || m_networkpool->activeThreadCount();
}

SyncPool* Manager::syncPool() const
{
    return m_syncpool;
}

Database* Manager::database() const
{
    return m_database;
}

NetworkPool* Manager::networkPool() const
{
    return m_networkpool;
}

ItemModel* Manager::itemModel(Manager::Item item) const
{
    return m_itemmodels.value(item);
}

void Manager::onActivityChanged()
{
    QTimer::singleShot(200, this, SIGNAL(isBusyChanged()));
}

void Manager::onLoaded(const QList<NotebookItem*>& notebooks,
                       const QList<ResourceItem*>& resources,
                       const QList<SearchItem*>& searches,
                       const QList<NoteItem*>& notes,
                       const QList<TagItem*>& tags)
{
    bool added = false;
    added |= m_itemmodels.value(Notebook)->add(notebooks);
    added |= m_itemmodels.value(Resource)->add(resources);
    added |= m_itemmodels.value(Search)->add(searches);
    added |= m_itemmodels.value(Note)->add(notes);
    added |= m_itemmodels.value(Tag)->add(tags);

    if (added)
        addNotes(m_itemmodels.value(Note)->items<NoteItem*>());
}

void Manager::onSynced(const QVector<evernote::edam::Notebook>& notebooks,
                       const QVector<evernote::edam::Resource>& resources,
                       const QVector<evernote::edam::SavedSearch>& searches,
                       const QVector<evernote::edam::Note>& notes,
                       const QVector<evernote::edam::Tag>& tags)
{
    bool added = false;
    QList<NotebookItem*> notebookItems;
    foreach (const evernote::edam::Notebook& notebook, notebooks)
        notebookItems += new NotebookItem(notebook, this);
    added |= m_itemmodels.value(Notebook)->add(notebookItems);

    QList<ResourceItem*> resourceItems;
    foreach (const evernote::edam::Resource& resource, resources)
        resourceItems += new ResourceItem(resource, this);
    added |= m_itemmodels.value(Resource)->add(resourceItems);

    QList<SearchItem*> searchItems;
    foreach (const evernote::edam::SavedSearch& search, searches)
        searchItems += new SearchItem(search, this);
    added |= m_itemmodels.value(Search)->add(searchItems);

    QList<NoteItem*> noteItems;
    foreach (const evernote::edam::Note& note, notes) {
        if (note.active)
            noteItems += new NoteItem(note, this);
        else if (NoteItem* noteItem = m_itemmodels.value(Note)->get<NoteItem*>(QString::fromStdString(note.guid))) {
            removeNotes(QList<NoteItem*>() << noteItem);
            m_itemmodels.value(Note)->remove(noteItem);
        }
    }
    added |= m_itemmodels.value(Note)->add(noteItems);

    QList<TagItem*> tagItems;
    foreach (const evernote::edam::Tag& tag, tags)
        tagItems += new TagItem(tag, this);
    added |= m_itemmodels.value(Tag)->add(tagItems);

    if (added)
        m_database->save(m_itemmodels.value(Notebook)->items<NotebookItem*>(),
                         m_itemmodels.value(Resource)->items<ResourceItem*>(),
                         m_itemmodels.value(Search)->items<SearchItem*>(),
                         m_itemmodels.value(Note)->items<NoteItem*>(),
                         m_itemmodels.value(Tag)->items<TagItem*>());

    if (!noteItems.isEmpty())
        addNotes(noteItems);
}

void Manager::onExpunged(const QVector<std::string>& notebooks,
                         const QVector<std::string>& searches,
                         const QVector<std::string>& notes,
                         const QVector<std::string>& tags)
{
    QList<NotebookItem*> notebookItems;
    foreach (const std::string& guid, notebooks) {
        NotebookItem* item = m_itemmodels.value(Notebook)->get<NotebookItem*>(QString::fromStdString(guid));
        if (item && m_itemmodels.value(Notebook)->remove(item))
            notebookItems += item;
    }

    QList<SearchItem*> searchItems;
    foreach (const std::string& guid, searches) {
        SearchItem* item = m_itemmodels.value(Search)->get<SearchItem*>(QString::fromStdString(guid));
        if (item && m_itemmodels.value(Search)->remove(item))
            searchItems += item;
    }

    QList<NoteItem*> noteItems;
    foreach (const std::string& guid, notes) {
        NoteItem* item = m_itemmodels.value(Note)->get<NoteItem*>(QString::fromStdString(guid));
        if (item && m_itemmodels.value(Note)->remove(item))
            noteItems += item;
    }

    QList<TagItem*> tagItems;
    foreach (const std::string& guid, tags) {
        TagItem* item = m_itemmodels.value(Tag)->get<TagItem*>(QString::fromStdString(guid));
        if (item && m_itemmodels.value(Tag)->remove(item))
            tagItems += item;
    }

    if (!notebookItems.isEmpty() || !searchItems.isEmpty() || !noteItems.isEmpty() || !tagItems.isEmpty())
        m_database->remove(notebookItems, searchItems, noteItems, tagItems);

    if (!noteItems.isEmpty())
        removeNotes(noteItems);
}

void Manager::onResourceFetched(const evernote::edam::Resource& resource)
{
    ResourceItem* item = m_itemmodels.value(Resource)->get<ResourceItem*>(QString::fromStdString(resource.guid));
    if (item) {
        item->setData(resource);
        m_files->write(item->guid(), item->filePath(false), QByteArray(resource.data.body.c_str(), resource.data.size));
        m_database->saveResource(item);
    }
}

void Manager::onNoteCreated(const evernote::edam::Note& note)
{
    // TODO: untested
    NoteItem* item = new NoteItem(note, this);
    m_itemmodels.value(Note)->add(item);
    m_database->saveNote(item);
    addNotes(QList<NoteItem*>() << item);
}

void Manager::onNoteFetched(const evernote::edam::Note& note)
{
    NoteItem* item = m_itemmodels.value(Note)->get<NoteItem*>(QString::fromStdString(note.guid));
    if (item) {
        item->setData(note);
        m_files->write(item->guid(), item->filePath(false), item->content());
        m_database->saveNote(item);

        for (uint i = 0; i < note.resources.size(); ++i)
            m_networkpool->fetchResource(note.resources.at(i));
    }
}

void Manager::onNoteMoved(const evernote::edam::Note& note)
{
    NoteItem* item = m_itemmodels.value(Note)->get<NoteItem*>(QString::fromStdString(note.guid));
    if (item) {
        foreach (NotebookItem* notebook, m_itemmodels.value(Notebook)->items<NotebookItem*>()) {
            if (notebook->notes()->remove(item)) {
                m_database->saveNotebook(notebook);
                break;
            }
        }
        NotebookItem* notebook = m_itemmodels.value(Notebook)->get<NotebookItem*>(QString::fromStdString(note.notebookGuid));
        if (notebook && notebook->notes()->add(item))
            m_database->saveNotebook(notebook);
        m_database->saveNote(item);
    }
}

void Manager::onNoteRenamed(const evernote::edam::Note& note)
{
    NoteItem* item = m_itemmodels.value(Note)->get<NoteItem*>(QString::fromStdString(note.guid));
    if (item) {
        item->setTitle(QString::fromStdString(note.title));
        m_database->saveNote(item);
    }
}

void Manager::onNoteTrashed(const evernote::edam::Note& note)
{
    NoteItem* item = m_itemmodels.value(Note)->get<NoteItem*>(QString::fromStdString(note.guid));
    if (item) {
        item->setData(note);
        m_database->saveNote(item);
    }
}

void Manager::onNotebookCreated(const evernote::edam::Notebook& notebook)
{
    // TODO: untested
    NotebookItem* item = new NotebookItem(notebook, this);
    m_itemmodels.value(Notebook)->add(item);
    m_database->saveNotebook(item);
}

void Manager::onNotebookFetched(const evernote::edam::Notebook& notebook)
{
    NotebookItem* item = m_itemmodels.value(Notebook)->get<NotebookItem*>(QString::fromStdString(notebook.guid));
    if (item) {
        item->setData(notebook);
        m_database->saveNotebook(item);
    }
}

void Manager::onNotebookRenamed(const evernote::edam::Notebook& notebook)
{
    // TODO: untested
    NotebookItem* item = m_itemmodels.value(Notebook)->get<NotebookItem*>(QString::fromStdString(notebook.guid));
    if (item) {
        item->setName(QString::fromStdString(notebook.name));
        m_database->saveNotebook(item);
    }
}

void Manager::onTagCreated(const evernote::edam::Tag& tag)
{
    // TODO: untested
    TagItem* item = new TagItem(tag, this);
    m_itemmodels.value(Notebook)->add(item);
    m_database->saveTag(item);
}

void Manager::onTagFetched(const evernote::edam::Tag& tag)
{
    // TODO: untested
    TagItem* item = m_itemmodels.value(Notebook)->get<TagItem*>(QString::fromStdString(tag.guid));
    if (item) {
        item->setData(tag);
        m_database->saveTag(item);
    }
}

void Manager::onTagRenamed(const evernote::edam::Tag& tag)
{
    // TODO: untested
    TagItem* item = m_itemmodels.value(Notebook)->get<TagItem*>(QString::fromStdString(tag.guid));
    if (item) {
        item->setName(QString::fromStdString(tag.name));
        m_database->saveTag(item);
    }
}

void Manager::onFileWritten(const QString& guid, const QString &filePath)
{
    if (m_itemmodels.value(Note)->contains(guid)) {
        NoteItem* note = m_itemmodels.value(Note)->get<NoteItem*>(guid);
        if (note)
            emit note->filePathChanged();
    } else if (m_itemmodels.value(Resource)->contains(guid)) {
        ResourceItem* resource = m_itemmodels.value(Resource)->get<ResourceItem*>(guid);
        if (resource) {
            if (QFileInfo(filePath).baseName() == "thumb")
                emit resource->thumbnailChanged();
            else
                emit resource->filePathChanged();
        }
    } else
        qCritical() << "### Manager::onFileWritten(): UNIDENTIFIED FILE:" << filePath;
}

void Manager::onThumbnailFetched(const QString& guid, const QByteArray& data)
{
    ResourceItem* resource = m_itemmodels.value(Resource)->get<ResourceItem*>(guid);
    if (resource)
        m_files->write(guid, resource->thumbnail(false), data);
    else
        qCritical() << "### Manager::onThumbnailFetched(): UNIDENTIFIED THUMBNAIL:" << guid;
}

void Manager::onSearchCreated(const evernote::edam::SavedSearch& search)
{
    // TODO: untested
    SearchItem* item = new SearchItem(search, this);
    m_itemmodels.value(Search)->add(item);
    m_database->saveSearch(item);
}

void Manager::onSearchFetched(const evernote::edam::SavedSearch& search)
{
    // TODO: untested
    SearchItem* item = m_itemmodels.value(Search)->get<SearchItem*>(QString::fromStdString(search.guid));
    if (item) {
        item->setData(search);
        m_database->saveSearch(item);
    }
}

void Manager::onSearchRenamed(const evernote::edam::SavedSearch& search)
{
    // TODO: untested
    SearchItem* item = m_itemmodels.value(Search)->get<SearchItem*>(QString::fromStdString(search.guid));
    if (item) {
        item->setName(QString::fromStdString(search.name));
        m_database->saveSearch(item);
    }
}

void Manager::onSearched(const evernote::edam::SavedSearch& search, const QVector<evernote::edam::NoteMetadata>& notes)
{
    SearchItem* searchItem = m_itemmodels.value(Search)->get<SearchItem*>(QString::fromStdString(search.guid));
    if (searchItem) {
        QList<NoteItem*> noteItems;
        foreach (const evernote::edam::NoteMetadata& note, notes) {
            NoteItem* noteItem = m_itemmodels.value(Note)->get<NoteItem*>(QString::fromStdString(note.guid));
            if (noteItem)
                noteItems += noteItem;
        }
        searchItem->notes()->add(noteItems);
    }
}

void Manager::addNotes(const QList<NoteItem*>& notes)
{
    foreach (NoteItem* note, notes) {
        const evernote::edam::Note& data = note->data();
        QString notebookGuid = QString::fromStdString(data.notebookGuid);
        NotebookItem* notebook = m_itemmodels.value(Notebook)->get<NotebookItem*>(notebookGuid);
        if (notebook)
            notebook->notes()->add(note);
        else
            qCritical() << "### Manager::addNotes(): MISSING NOTEBOOK:" << notebookGuid;

        for (uint i = 0; i < data.resources.size(); ++i) {
            QString resourceGuid = QString::fromStdString(data.resources.at(i).guid);
            ResourceItem* resource = m_itemmodels.value(Resource)->get<ResourceItem*>(resourceGuid);
            if (resource)
                note->resources()->add(resource);
            else
                qCritical() << "### Manager::addNotes(): MISSING RESOURCE:" << resourceGuid;
        }

        for (uint i = 0; i < data.tagGuids.size(); ++i) {
            QString tagGuid = QString::fromStdString(data.tagGuids.at(i));
            TagItem* tag = m_itemmodels.value(Tag)->get<TagItem*>(tagGuid);
            if (tag) {
                note->tags()->add(tag);
                tag->notes()->add(note);
            } else
                qCritical() << "### Manager::addNotes(): MISSING TAG:" << tagGuid;
        }
    }
}

void Manager::removeNotes(const QList<NoteItem*>& notes)
{
    foreach (NoteItem* note, notes) {
        const evernote::edam::Note& data = note->data();
        QString notebookGuid = QString::fromStdString(data.notebookGuid);
        NotebookItem* notebook = m_itemmodels.value(Notebook)->get<NotebookItem*>(notebookGuid);
        if (notebook)
            notebook->notes()->remove(note);
        else
            qCritical() << "### Manager::removeNotes(): MISSING NOTEBOOK:" << notebookGuid;

        for (uint i = 0; i < data.resources.size(); ++i) {
            QString resourceGuid = QString::fromStdString(data.resources.at(i).guid);
            ResourceItem* resource = m_itemmodels.value(Resource)->get<ResourceItem*>(resourceGuid);
            if (resource)
                note->resources()->remove(resource);
            else
                qCritical() << "### Manager::removeNotes(): MISSING RESOURCE:" << resourceGuid;
        }

        for (uint i = 0; i < data.tagGuids.size(); ++i) {
            QString tagGuid = QString::fromStdString(data.tagGuids.at(i));
            TagItem* tag = m_itemmodels.value(Tag)->get<TagItem*>(tagGuid);
            if (tag) {
                note->tags()->remove(tag);
                tag->notes()->remove(note);
            } else
                qCritical() << "### Manager::removeNotes(): MISSING TAG:" << tagGuid;
        }
    }
}
