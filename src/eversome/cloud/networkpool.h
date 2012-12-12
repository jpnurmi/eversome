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
#ifndef NETWORKPOOL_H
#define NETWORKPOOL_H

#include "abstractpool.h"

class Session;
class NetworkOperation;

class NetworkPool : public AbstractPool
{
    Q_OBJECT

public:
    explicit NetworkPool(Session* session);
    virtual ~NetworkPool();

    Session* session() const;

protected:
    using AbstractPool::startOperation;
    void startOperation(NetworkOperation* operation, const QString& path);

    Session* m_session;
};

#endif // NETWORKPOOL_H
