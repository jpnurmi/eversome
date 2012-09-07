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
#ifndef NETWORKOPERATION_H
#define NETWORKOPERATION_H

#include "operation.h"
#include <thrift/protocol/TProtocol.h>

class NetworkOperation : public Operation
{
    Q_OBJECT

public:
    NetworkOperation(Mode mode);
    ~NetworkOperation();

    bool isValid() const;

    QString host() const;
    void setHost(const QString& host);

    int port() const;
    void setPort(int port);

    QString path() const;
    void setPath(const QString& path);

    QString authToken() const;
    void setAuthToken(const QString& token);

protected:
    virtual void operate();
    virtual void operate(boost::shared_ptr<apache::thrift::protocol::TProtocol> protocol) = 0;

private:
    int m_port;
    QString m_host;
    QString m_path;
    QString m_token;
};

#endif // NETWORKOPERATION_H
