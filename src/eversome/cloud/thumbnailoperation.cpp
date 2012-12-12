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
//#define QT_NO_DEBUG_OUTPUT

#include "thumbnailoperation.h"
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QEventLoop>

ThumbnailOperation::ThumbnailOperation(const QString& guid) :
    NetworkOperation(NetworkOperation::FetchThumbnail), m_guid(guid)
{
}

ThumbnailOperation::~ThumbnailOperation()
{
}

bool ThumbnailOperation::isValid() const
{
    return NetworkOperation::isValid(); // TODO
}

void ThumbnailOperation::operate()
{
    QNetworkAccessManager manager;
    QNetworkRequest request;
    request.setUrl(url());
    QByteArray data = QString("size=96&auth=%1").arg(authToken()).toUtf8();
    QNetworkReply* reply = manager.post(request, data);

    QEventLoop loop;
    connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
    connect(reply, SIGNAL(error(QNetworkReply::NetworkError)), &loop, SLOT(quit()));
    loop.exec();

    if (reply->isReadable())
        emit fetched(m_guid, reply->readAll());

    /*
    if (operation)
        qDebug() << "DatabaseOperation::operate():" << operation
                 << "NB:" << m_notebooks.count()
                 << "R:" << m_resources.count()
                 << "S:" << m_searches.count()
                 << "N:" << m_notes.count()
                 << "T:" << m_tags.count();
    */
}
