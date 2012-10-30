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

#include <QDebug>
#include <QObject>
#include <QString>
#include <QMetaType>
#include <QRunnable>

class Operation : public QObject, public QRunnable
{
    Q_OBJECT
    Q_ENUMS(Mode)

public:
    enum Mode
    {
        // file operations
        ReadFile,
        WriteFile,
        GenerateThumbnail,

        // database operations
        OpenDatabase,
        CloseDatabase,
        ResetDatabase,
        LoadDatabase,
        SaveDatabase,
        RemoveDatabase,

        // store operations
        Sync,

        // search operations
        CreateSearch,
        FetchSearch,
        PerformSearch,
        RenameSearch,

        // note operations
        CreateNote,
        FetchNote,
        MoveNote,
        RenameNote,
        TrashNote,

        // notebook operations
        CreateNotebook,
        FetchNotebook,
        RenameNotebook,

        // tag operations
        CreateTag,
        FetchTag,
        RenameTag,

        // resource operations
        FetchResource
    };

    Operation(Mode mode);
    ~Operation();

    Mode mode() const;
    virtual bool isValid() const = 0;
    virtual void run();

signals:
    void started();
    void finished();
    void error(const QString& error);

protected:
    virtual void operate() = 0;

private:
    Mode m_mode;
};

QDebug operator<<(QDebug debug, const Operation* operation);

#endif // OPERATION_H
