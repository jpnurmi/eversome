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

#include "operation.h"
#include <Types_types.h>

class NotebookOperation : public Operation
{
    Q_OBJECT

public:
    NotebookOperation(const evernote::edam::Notebook& notebook, Mode mode);
    ~NotebookOperation();

    evernote::edam::Notebook notebook() const;

    void operate(boost::shared_ptr<apache::thrift::protocol::TProtocol> protocol);

private:
    evernote::edam::Notebook m_notebook;
};

#endif // NOTEBOOKOPERATION_H
