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

#include "notebookoperation.h"
#include <NoteStore.h>
#include <QtDebug>

using namespace boost;
using namespace apache;
using namespace evernote;

NotebookOperation::NotebookOperation(const edam::Notebook& notebook, Mode mode) :
    Operation(mode), m_notebook(notebook)
{
}

NotebookOperation::~NotebookOperation()
{
}

edam::Notebook NotebookOperation::notebook() const
{
    return m_notebook;
}

void NotebookOperation::operate(shared_ptr<thrift::protocol::TProtocol> protocol)
{
    int usn = 0; // TODO
    edam::NoteStoreClient client(protocol);
    std::string token = authToken().toStdString();
    switch (mode())
    {
        case CreateNotebook:
            client.createNotebook(m_notebook, token, m_notebook);
            break;
        case ExpungeNotebook:
            usn = client.expungeNotebook(token, m_notebook.guid);
            break;
        case GetDefaultNotebook:
            client.getDefaultNotebook(m_notebook, token);
            break;
        case UpdateNotebook:
            usn = client.updateNotebook(token, m_notebook);
            break;
        default:
            qWarning() << Q_FUNC_INFO << "unknown mode" << mode();
            break;
    }
}