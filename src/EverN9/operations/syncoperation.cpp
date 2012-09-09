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

void SyncOperation::operate(shared_ptr<thrift::protocol::TProtocol> protocol)
{
    if (mode() != Sync) {
        qWarning() << Q_FUNC_INFO << "unknown mode" << mode();
        return;
    }

    edam::SyncChunk chunk;
    edam::NoteStoreClient client(protocol);
    std::string token = authToken().toStdString();

    // TODO: report progress

    do {
        client.getSyncChunk(chunk, token, m_usn, 256, false);

        if (m_usn < chunk.updateCount)
            m_usn = chunk.chunkHighUSN;

        qDebug() << Q_FUNC_INFO << "synced" << m_usn
                 << "NB:" << chunk.notebooks.size()
                 << "R:" << chunk.resources.size()
                 << "S:" << chunk.searches.size()
                 << "N:" << chunk.notes.size()
                 << "T:" << chunk.tags.size();

        emit synced(QVector<edam::Notebook>::fromStdVector(chunk.notebooks),
                    QVector<edam::Resource>::fromStdVector(chunk.resources),
                    QVector<edam::SavedSearch>::fromStdVector(chunk.searches),
                    QVector<edam::Note>::fromStdVector(chunk.notes),
                    QVector<edam::Tag>::fromStdVector(chunk.tags));

        qDebug() << Q_FUNC_INFO << "expunged" << m_usn
                 << "NB:" << chunk.expungedNotebooks.size()
                 << "S:" << chunk.expungedSearches.size()
                 << "N:" << chunk.expungedNotes.size()
                 << "T:" << chunk.expungedTags.size();

        emit expunged(QVector<std::string>::fromStdVector(chunk.expungedNotebooks),
                      QVector<std::string>::fromStdVector(chunk.expungedSearches),
                      QVector<std::string>::fromStdVector(chunk.expungedNotes),
                      QVector<std::string>::fromStdVector(chunk.expungedTags));

        emit usnChanged(m_usn);
        emit currentTimeChanged(QDateTime::fromMSecsSinceEpoch(chunk.currentTime));

    } while (m_usn < chunk.updateCount);
}
