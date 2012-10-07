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
#ifndef NOTEBOOKOPERATION_H
#define NOTEBOOKOPERATION_H

#include "networkoperation.h"
#include <Types_types.h>

class NotebookOperation : public NetworkOperation
{
    Q_OBJECT

public:
    NotebookOperation(const evernote::edam::Notebook& notebook, Mode mode);
    ~NotebookOperation();

signals:
    void notebookCreated(const evernote::edam::Notebook& notebook);
    void notebookFetched(const evernote::edam::Notebook& notebook);
    void defaultNotebookFetched(const evernote::edam::Notebook& notebook);
    void notebookUpdated(const evernote::edam::Notebook& notebook);

protected:
    void operate(boost::shared_ptr<apache::thrift::protocol::TProtocol> protocol);

private:
    evernote::edam::Notebook m_notebook;
};

#endif // NOTEBOOKOPERATION_H
