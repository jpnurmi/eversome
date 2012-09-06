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
#ifndef BASEOPERATION_H
#define BASEOPERATION_H

#include <QString>
#include <QObject>
#include <QMetaType>
#include <QRunnable>
#include <thrift/protocol/TProtocol.h>

class BaseOperation : public QObject, public QRunnable
{
    Q_OBJECT
    Q_ENUMS(Operation)

public:
    enum Operation
    {
        CreateNote,
        DeleteNote,
        GetNote,
        ExpungeNote,
        ShareNote,
        UnshareNote,
        UpdateNote
    };

    BaseOperation(Operation operation);
    ~BaseOperation();

    Operation operation() const;
    bool isValid() const;

    QString host() const;
    void setHost(const QString& host);

    int port() const;
    void setPort(int port);

    QString path() const;
    void setPath(const QString& path);

    QString authToken() const;
    void setAuthToken(const QString& token);

    virtual void run();

signals:
    void started(BaseOperation* operation);
    void finished(BaseOperation* operation);
    void error(BaseOperation* operation, const QString& error);

protected:
    virtual void operate(boost::shared_ptr<apache::thrift::protocol::TProtocol> protocol) = 0;

private:
    int m_port;
    QString m_host;
    QString m_path;
    QString m_token;
    Operation m_operation;
};

Q_DECLARE_METATYPE(BaseOperation*)

QDebug operator<<(QDebug debug, const BaseOperation* operation);

#endif // BASEOPERATION_H
