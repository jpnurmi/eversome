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

#include "abstractoperation.h"

AbstractOperation::AbstractOperation()
{
    setAutoDelete(true);
}

AbstractOperation::~AbstractOperation()
{
}

void AbstractOperation::run()
{
    if (!isValid()) {
        qCritical() << Q_FUNC_INFO << "INVALID:" << this;
        return;
    }

    emit started();

    operate();

    emit finished();
}

QDebug operator<<(QDebug debug, const AbstractOperation* operation)
{
    if (!operation)
        return debug << "AbstractOperation(0x0) ";

    const QMetaObject* metaObject = operation->metaObject();
    debug.nospace() << metaObject->className() << '(' << (void*) operation;
    if (!operation->objectName().isEmpty())
        debug << ", name = " << operation->objectName();

    debug << ", valid = " << operation->isValid() << ')';
    return debug.space();
}
