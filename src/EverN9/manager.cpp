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
#include "notestore.h"
#include "filesystem.h"
#include "notebookitem.h"
#include "resourceitem.h"
#include "searchitem.h"
#include "itemmodel.h"
#include "noteitem.h"
#include "tagitem.h"
#include <QThreadPool>
#include <QFileInfo>
#include <QVector>
#include <QDebug>

Q_DECLARE_METATYPE(QVector<std::string>)

Manager::Manager(Session* session) : QObject(session)
{
    m_store = new NoteStore(session);

    connect(m_store, SIGNAL(synced(QVector<evernote::edam::Notebook>,
                                   QVector<evernote::edam::Resource>,
                                   QVector<evernote::edam::SavedSearch>,
                                   QVector<evernote::edam::Note>,
                                   QVector<evernote::edam::Tag>)),
               this, SLOT(onSynced(QVector<evernote::edam::Notebook>,
                                   QVector<evernote::edam::Resource>,
                                   QVector<evernote::edam::SavedSearch>,
                                   QVector<evernote::edam::Note>,
                                   QVector<evernote::edam::Tag>)), Qt::QueuedConnection);

    qRegisterMetaType<QVector<std::string> >();
    connect(m_store, SIGNAL(expunged(QVector<std::string>,QVector<std::string>,
                                     QVector<std::string>,QVector<std::string>)),
               this, SLOT(onExpunged(QVector<std::string>,QVector<std::string>,
                                     QVector<std::string>,QVector<std::string>)), Qt::QueuedConnection);

    connect(m_store, SIGNAL(resourceFetched(evernote::edam::Resource)),
               this, SLOT(onResourceFetched(evernote::edam::Resource)), Qt::QueuedConnection);
    connect(m_store, SIGNAL(noteFetched(evernote::edam::Note)),
               this, SLOT(onNoteFetched(evernote::edam::Note)), Qt::QueuedConnection);
    connect(m_store, SIGNAL(searched(evernote::edam::SavedSearch,QVector<evernote::edam::Note>)),
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

    m_files = new FileSystem(this);
    connect(m_files, SIGNAL(written(QString)), SLOT(onFileWritten(QString)), Qt::QueuedConnection);

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
    m_database->open();
    m_database->load(this);

    connect(m_store, SIGNAL(activityChanged()), this, SIGNAL(isBusyChanged()));
    connect(m_files, SIGNAL(activityChanged()), this, SIGNAL(isBusyChanged()));
    connect(m_database, SIGNAL(activityChanged()), this, SIGNAL(isBusyChanged()));

    m_notebooks = new ItemModel(this);
    m_resources = new ItemModel(this);
    m_searches = new ItemModel(this);
    m_notes = new ItemModel(this);
    m_tags = new ItemModel(this);
}

Manager::~Manager()
{
    m_notebooks->clear();
    m_resources->clear();
    m_searches->clear();
    m_notes->clear();
    m_tags->clear();
    m_database->close();
    QThreadPool::globalInstance()->waitForDone();
}

bool Manager::isBusy() const
{
    return QThreadPool::globalInstance()->activeThreadCount();
}

NoteStore* Manager::noteStore() const
{
    return m_store;
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

void Manager::onLoaded(const QList<NotebookItem*>& notebooks,
                       const QList<ResourceItem*>& resources,
                       const QList<SearchItem*>& searches,
                       const QList<NoteItem*>& notes,
                       const QList<TagItem*>& tags)
{
    bool added = false;
    added |= m_notebooks->add(notebooks);
    added |= m_resources->add(resources);
    added |= m_searches->add(searches);
    added |= m_notes->add(notes);
    added |= m_tags->add(tags);

    if (added)
        addNotes(m_notes->items<NoteItem*>());
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
    added |= m_notebooks->add(notebookItems);

    QList<ResourceItem*> resourceItems;
    foreach (const evernote::edam::Resource& resource, resources)
        resourceItems += new ResourceItem(resource, this);
    added |= m_resources->add(resourceItems);

    QList<SearchItem*> searchItems;
    foreach (const evernote::edam::SavedSearch& search, searches)
        searchItems += new SearchItem(search, this);
    added |= m_searches->add(searchItems);

    QList<NoteItem*> noteItems;
    foreach (const evernote::edam::Note& note, notes)
        noteItems += new NoteItem(note, this);
    added |= m_notes->add(noteItems);

    QList<TagItem*> tagItems;
    foreach (const evernote::edam::Tag& tag, tags)
        tagItems += new TagItem(tag, this);
    added |= m_tags->add(tagItems);

    if (added)
        m_database->save(m_notebooks->items<NotebookItem*>(),
                         m_resources->items<ResourceItem*>(),
                         m_searches->items<SearchItem*>(),
                         m_notes->items<NoteItem*>(),
                         m_tags->items<TagItem*>());

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
        NotebookItem* item = m_notebooks->get<NotebookItem*>(QString::fromStdString(guid));
        if (item && m_notebooks->remove(item))
            notebookItems += item;
    }

    QList<SearchItem*> searchItems;
    foreach (const std::string& guid, searches) {
        SearchItem* item = m_searches->get<SearchItem*>(QString::fromStdString(guid));
        if (item && m_searches->remove(item))
            searchItems += item;
    }

    QList<NoteItem*> noteItems;
    foreach (const std::string& guid, notes) {
        NoteItem* item = m_notes->get<NoteItem*>(QString::fromStdString(guid));
        if (item && m_notes->remove(item))
            noteItems += item;
    }

    QList<TagItem*> tagItems;
    foreach (const std::string& guid, tags) {
        TagItem* item = m_tags->get<TagItem*>(QString::fromStdString(guid));
        if (item && m_tags->remove(item))
            tagItems += item;
    }

    if (!notebookItems.isEmpty() || !searchItems.isEmpty() || !noteItems.isEmpty() || !tagItems.isEmpty())
        m_database->remove(notebookItems, searchItems, noteItems, tagItems);

    if (!noteItems.isEmpty())
        removeNotes(noteItems);
}

void Manager::onResourceFetched(const evernote::edam::Resource& resource)
{
    ResourceItem* item = m_resources->get<ResourceItem*>(QString::fromStdString(resource.guid));
    if (item)
        m_files->write(item->filePath(false), QByteArray(resource.data.body.c_str(), resource.data.size));
}

void Manager::onNoteFetched(const evernote::edam::Note& note)
{
    NoteItem* item = m_notes->get<NoteItem*>(QString::fromStdString(note.guid));
    if (item)
        m_files->write(item->filePath(false), QByteArray(note.content.c_str(), note.content.size()));
}

void Manager::onFileWritten(const QString &filePath)
{
    QString guid = QFileInfo(filePath).baseName();
    if (m_notes->contains(guid)) {
        NoteItem* note = m_notes->get<NoteItem*>(guid);
        if (note)
            note->update();
    } else if (m_resources->contains(guid)) {
        ResourceItem* resource = m_resources->get<ResourceItem*>(guid);
        if (resource)
            resource->update();
    } else
        qCritical() << "### Manager::onFileWritten(): UNIDENTIFIED FILE:" << filePath;
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

void Manager::addNotes(const QList<NoteItem*>& notes)
{
    foreach (NoteItem* note, notes) {
        const evernote::edam::Note& data = note->data();
        QString notebookGuid = QString::fromStdString(data.notebookGuid);
        NotebookItem* notebook = m_notebooks->get<NotebookItem*>(notebookGuid);
        if (notebook)
            notebook->notes()->add(note);
        else
            qCritical() << "### Manager::addNotes(): MISSING NOTEBOOK:" << notebookGuid;

        for (uint i = 0; i < data.resources.size(); ++i) {
            QString resourceGuid = QString::fromStdString(data.resources.at(i).guid);
            ResourceItem* resource = m_resources->get<ResourceItem*>(resourceGuid);
            if (resource)
                note->resources()->add(resource);
            else
                qCritical() << "### Manager::addNotes(): MISSING RESOURCE:" << resourceGuid;
        }

        for (uint i = 0; i < data.tagGuids.size(); ++i) {
            QString tagGuid = QString::fromStdString(data.tagGuids.at(i));
            TagItem* tag = m_tags->get<TagItem*>(tagGuid);
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
        NotebookItem* notebook = m_notebooks->get<NotebookItem*>(notebookGuid);
        if (notebook)
            notebook->notes()->remove(note);
        else
            qCritical() << "### Manager::removeNotes(): MISSING NOTEBOOK:" << notebookGuid;

        for (uint i = 0; i < data.resources.size(); ++i) {
            QString resourceGuid = QString::fromStdString(data.resources.at(i).guid);
            ResourceItem* resource = m_resources->get<ResourceItem*>(resourceGuid);
            if (resource)
                note->resources()->remove(resource);
            else
                qCritical() << "### Manager::removeNotes(): MISSING RESOURCE:" << resourceGuid;
        }

        for (uint i = 0; i < data.tagGuids.size(); ++i) {
            QString tagGuid = QString::fromStdString(data.tagGuids.at(i));
            TagItem* tag = m_tags->get<TagItem*>(tagGuid);
            if (tag) {
                note->tags()->remove(tag);
                tag->notes()->remove(note);
            } else
                qCritical() << "### Manager::removeNotes(): MISSING TAG:" << tagGuid;
        }
    }
}
