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
#ifndef NOTEBOOKITEM_H
#define NOTEBOOKITEM_H

#include <QObject>
#include <QDateTime>
#include <QMetaType>
#include <Types_types.h>
#include "itemmodel.h"

class NotebookItem : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString guid READ guid CONSTANT)
    Q_PROPERTY(QString name READ name CONSTANT)
    Q_PROPERTY(bool isDefault READ isDefault CONSTANT)
    Q_PROPERTY(bool isPublished READ isPublished CONSTANT)
    Q_PROPERTY(QDateTime created READ created CONSTANT)
    Q_PROPERTY(QDateTime updated READ updated CONSTANT)
    Q_PROPERTY(ItemModel* notes READ notes CONSTANT)
    Q_PROPERTY(int usn READ usn CONSTANT)

public:
    explicit NotebookItem(evernote::edam::Notebook notebook = evernote::edam::Notebook(), QObject* parent = 0);
    virtual ~NotebookItem();

    evernote::edam::Notebook notebook() const;

    QString guid() const;
    QString name() const;
    bool isDefault() const;
    bool isPublished() const;
    QDateTime created() const;
    QDateTime updated() const;
    ItemModel* notes() const;
    int usn() const;

private:
    ItemModel* m_notes;
    evernote::edam::Notebook m_notebook;
};

Q_DECLARE_METATYPE(NotebookItem*)
Q_DECLARE_METATYPE(QList<NotebookItem*>)

#endif // NOTEBOOKITEM_H
