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

const edam::SyncChunk& SyncOperation::chunk() const
{
    return m_chunk;
}

void SyncOperation::operate(shared_ptr<thrift::protocol::TProtocol> protocol)
{
    if (mode() != Sync) {
        qWarning() << Q_FUNC_INFO << "unknown mode" << mode();
        return;
    }

    edam::NoteStoreClient client(protocol);
    std::string token = authToken().toStdString();

    // TODO: report progress

    do {
        edam::SyncChunk chunk;
        client.getSyncChunk(chunk, token, m_usn, 256, false);

        if (m_usn < chunk.updateCount)
            m_usn = chunk.chunkHighUSN;

        m_chunk.updateCount = chunk.updateCount;
        m_chunk.currentTime = chunk.currentTime;
        m_chunk.notebooks.insert(m_chunk.notebooks.end(), chunk.notebooks.begin(), chunk.notebooks.end());
        m_chunk.resources.insert(m_chunk.resources.end(), chunk.resources.begin(), chunk.resources.end());
        m_chunk.searches.insert(m_chunk.searches.end(), chunk.searches.begin(), chunk.searches.end());
        m_chunk.notes.insert(m_chunk.notes.end(), chunk.notes.begin(), chunk.notes.end());
        m_chunk.tags.insert(m_chunk.tags.end(), chunk.tags.begin(), chunk.tags.end());

        qDebug() << Q_FUNC_INFO
                 << "NB:" << chunk.notebooks.size()
                 << "R:" << chunk.resources.size()
                 << "S:" << chunk.searches.size()
                 << "N:" << chunk.notes.size()
                 << "T:" << chunk.tags.size()
                 << "USN:" << m_usn;

    } while (m_usn < m_chunk.updateCount);
}
