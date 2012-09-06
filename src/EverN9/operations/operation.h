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
#ifndef OPERATION_H
#define OPERATION_H

#include <QString>
#include <QObject>
#include <QMetaType>
#include <QRunnable>
#include <thrift/protocol/TProtocol.h>

class Operation : public QObject, public QRunnable
{
    Q_OBJECT
    Q_ENUMS(Mode)

public:
    enum Mode
    {
        Auth,
        Sync,
        Search,

        CreateNote,
        DeleteNote,
        GetNote,
        ExpungeNote,
        ShareNote,
        UnshareNote,
        UpdateNote,

        CreateNotebook,
        ExpungeNotebook,
        GetDefaultNotebook,
        UpdateNotebook
    };

    Operation(Mode mode);
    ~Operation();

    Mode mode() const;
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
    void started(Operation* operation);
    void finished(Operation* operation);
    void error(Operation* operation, const QString& error);

protected:
    virtual void operate(boost::shared_ptr<apache::thrift::protocol::TProtocol> protocol) = 0;

private:
    int m_port;
    Mode m_mode;
    QString m_host;
    QString m_path;
    QString m_token;
};

Q_DECLARE_METATYPE(Operation*)

QDebug operator<<(QDebug debug, const Operation* operation);

#endif // OPERATION_H
