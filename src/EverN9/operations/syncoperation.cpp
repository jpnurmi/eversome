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
//#define QT_NO_DEBUG_OUTPUT

#include "syncoperation.h"
#include <NoteStore.h>

using namespace boost;
using namespace apache;
using namespace evernote;

SyncOperation::SyncOperation(int usn) : NetworkOperation(Sync), m_usn(usn)
{
}

SyncOperation::~SyncOperation()
{
}

int SyncOperation::usn() const
{
    return m_usn;
}

QDateTime SyncOperation::currentTime() const
{
    return m_time;
}

QVector<edam::Notebook> SyncOperation::notebooks() const
{
    return m_notebooks;
}

QVector<edam::Resource> SyncOperation::resources() const
{
    return m_resources;
}

QVector<edam::SavedSearch> SyncOperation::searches() const
{
    return m_searches;
}

QVector<edam::Note> SyncOperation::notes() const
{
    return m_notes;
}

QVector<edam::Tag> SyncOperation::tags() const
{
    return m_tags;
}

void SyncOperation::operate(shared_ptr<thrift::protocol::TProtocol> protocol)
{
    if (mode() != Sync) {
        qWarning() << Q_FUNC_INFO << "unknown mode" << mode();
        return;
    }

    edam::NoteStoreClient client(protocol);
    std::string token = authToken().toStdString();
    edam::SyncChunk chunk;

    // TODO: report progress

    do {
        client.getSyncChunk(chunk, token, m_usn, 256, false);

        if (m_usn < chunk.updateCount)
            m_usn = chunk.chunkHighUSN;

        m_time = QDateTime::fromMSecsSinceEpoch(chunk.currentTime);
        m_notebooks += QVector<edam::Notebook>::fromStdVector(chunk.notebooks);
        m_resources += QVector<edam::Resource>::fromStdVector(chunk.resources);
        m_searches += QVector<edam::SavedSearch>::fromStdVector(chunk.searches);
        m_notes += QVector<edam::Note>::fromStdVector(chunk.notes);
        m_tags += QVector<edam::Tag>::fromStdVector(chunk.tags);

        qDebug() << Q_FUNC_INFO
                 << "NB:" << chunk.notebooks.size()
                 << "R:" << chunk.resources.size()
                 << "S:" << chunk.searches.size()
                 << "N:" << chunk.notes.size()
                 << "T:" << chunk.tags.size()
                 << "USN:" << m_usn;

    } while (m_usn < chunk.updateCount);
}
