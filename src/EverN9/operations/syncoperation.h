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
#ifndef SYNCOPERATION_H
#define SYNCOPERATION_H

#include "networkoperation.h"
#include <NoteStore_types.h>

class SyncOperation : public NetworkOperation
{
    Q_OBJECT

public:
    SyncOperation(int usn);
    ~SyncOperation();

    int usn() const;
    const evernote::edam::SyncChunk& chunk() const;

protected:
    void operate(boost::shared_ptr<apache::thrift::protocol::TProtocol> protocol);

private:
    int m_usn;
    evernote::edam::SyncChunk m_chunk;
};

#endif // SYNCOPERATION_H
