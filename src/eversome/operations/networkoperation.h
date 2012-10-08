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
#include <QUrl>

class NetworkOperation : public Operation
{
    Q_OBJECT

public:
    NetworkOperation(Mode mode);
    ~NetworkOperation();

    bool isValid() const;

    QUrl url() const;
    void setUrl(const QUrl& url);

    QString authToken() const;
    void setAuthToken(const QString& token);

protected:
    virtual void operate();
    virtual void operate(boost::shared_ptr<apache::thrift::protocol::TProtocol> protocol) = 0;

private:
    QUrl m_url;
    QString m_token;
};

#endif // NETWORKOPERATION_H