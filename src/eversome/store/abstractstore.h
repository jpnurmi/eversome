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
#ifndef ABSTRACTSTORE_H
#define ABSTRACTSTORE_H

#include <QObject>

class Session;
class NetworkOperation;

class AbstractStore : public QObject
{
    Q_OBJECT

public:
    explicit AbstractStore(Session* session);
    virtual ~AbstractStore();

    Session* session() const;

signals:
    void activityChanged();
    void error(const QString& error);

protected:
    void startOperation(NetworkOperation* operation);

    Session* m_session;
};

#endif // ABSTRACTSTORE_H
