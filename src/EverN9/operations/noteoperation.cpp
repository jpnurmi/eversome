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

using namespace boost;
using namespace apache;
using namespace evernote;

NoteOperation::NoteOperation(const edam::Note& note, Mode mode) :
    NetworkOperation(mode), m_note(note)
{
}

NoteOperation::~NoteOperation()
{
}

void NoteOperation::operate(shared_ptr<thrift::protocol::TProtocol> protocol)
{
    int usn = 0; // TODO
    std::string key; // TODO
    edam::NoteStoreClient client(protocol);
    std::string token = authToken().toStdString();
    switch (mode())
    {
        case CreateNote:
            client.createNote(m_note, token, m_note);
            break;
        case DeleteNote:
            usn = client.deleteNote(token, m_note.guid);
            break;
        case GetNote:
            client.getNoteContent(m_note.content, token, m_note.guid);
            emit noteFetched(m_note);
            for (uint i = 0; i < m_note.resources.size(); ++i) {
                edam::Resource resource = m_note.resources.at(i);
                client.getResource(resource, token, resource.guid, true, false, false, false);
                emit resourceFetched(resource);
            }
            break;
        case ExpungeNote:
            usn = client.expungeNote(token, m_note.guid);
            break;
        case ShareNote:
            client.shareNote(key, token, m_note.guid);
            break;
        case UnshareNote:
            client.stopSharingNote(token, m_note.guid);
            break;
        case UpdateNote:
            client.updateNote(m_note, token, m_note);
            break;
        default:
            qWarning() << Q_FUNC_INFO << "unknown mode" << mode();
            break;
    }
}
