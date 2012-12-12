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
#ifndef THUMBNAILOPERATION_H
#define THUMBNAILOPERATION_H

#include "networkoperation.h"
#include <Types_types.h>

class ThumbnailOperation : public NetworkOperation
{
    Q_OBJECT

public:
    ThumbnailOperation(const evernote::edam::Note& note);
    ThumbnailOperation(const evernote::edam::Resource& resource);
    ~ThumbnailOperation();

    bool isValid() const;

signals:
    void fetched(const evernote::edam::Note& note);
    void fetched(const evernote::edam::Resource& resource);

protected:
    void operate();

private:
    evernote::edam::Note m_note;
    evernote::edam::Resource m_resource;
};

#endif // THUMBNAILOPERATION_H
