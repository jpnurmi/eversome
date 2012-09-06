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

#include "noteoperation.h"
#include <NoteStore.h>
#include <QtDebug>

using namespace boost;
using namespace apache;
using namespace evernote;

NoteOperation::NoteOperation(const evernote::edam::Note& note, Operation operation)
    : m_note(note), m_operation(operation)
{
}

NoteOperation::~NoteOperation()
{
}

evernote::edam::Note NoteOperation::note() const
{
    return m_note;
}

NoteOperation::Operation NoteOperation::operation() const
{
    return m_operation;
}


void NoteOperation::operate(shared_ptr<thrift::protocol::TProtocol> protocol)
{
    int usn = 0; // TODO
    std::string key; // TODO
    edam::NoteStoreClient client(protocol);
    std::string token = authToken().toStdString();
    switch (m_operation)
    {
        case Create:
            client.createNote(m_note, token, m_note);
            break;
        case Delete:
            usn = client.deleteNote(token, m_note.guid);
            break;
        case Get:
            client.getNoteContent(m_note.content, token, m_note.guid);
            for (uint i = 0; i < m_note.resources.size(); ++i)
                client.getResource(m_note.resources[i], token, m_note.resources.at(i).guid, true, false, false, false);
            break;
        case Expunge:
            usn = client.expungeNote(token, m_note.guid);
            break;
        case Share:
            client.shareNote(key, token, m_note.guid);
            break;
        case Unshare:
            client.stopSharingNote(token, m_note.guid);
            break;
        case Update:
            client.updateNote(m_note, token, m_note);
            break;
        default:
            qWarning() << Q_FUNC_INFO << "unknown operation" << m_operation;
            break;
    }
}
