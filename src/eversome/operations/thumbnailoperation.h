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

class ThumbnailOperation : public NetworkOperation
{
    Q_OBJECT

public:
    ThumbnailOperation(const QString& guid);
    ~ThumbnailOperation();

    bool isValid() const;

signals:
    void fetched(const QString& guid, const QByteArray& data);

protected:
    void operate();

private:
    QString m_guid;
};

#endif // THUMBNAILOPERATION_H
