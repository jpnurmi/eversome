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
#ifndef DATABASE_H
#define DATABASE_H

#include <QList>
#include <QObject>
#include "operation.h"

class TagItem;
class NoteItem;
class SearchItem;
class ResourceItem;
class NotebookItem;
class DatabaseOperation;

class Database : public QObject
{
    Q_OBJECT

public:
    explicit Database(QObject* parent = 0);
    virtual ~Database();

    void load(QObject* parent = 0);
    void save(const QList<NotebookItem*>& notebooks,
              const QList<ResourceItem*>& resources,
              const QList<SearchItem*>& searches,
              const QList<NoteItem*>& notes,
              const QList<TagItem*>& tags);

public slots:
    void open();
    void close();
    void reset();

signals:
    void activityChanged();
    void error(const QString& error);
    void loaded(const QList<NotebookItem*>& notebooks,
                const QList<ResourceItem*>& resources,
                const QList<SearchItem*>& searches,
                const QList<NoteItem*>& notes,
                const QList<TagItem*>& tags);

private slots:
    void onOperationStarted(Operation* operation);
    void onOperationFinished(Operation* operation);
    void onOperationError(Operation* operation, const QString& error);

private:
    DatabaseOperation* createOperation(Operation::Mode mode, QObject* parent = 0);
};

#endif // DATABASE_H
