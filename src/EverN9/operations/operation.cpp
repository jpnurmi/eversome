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

#include "operation.h"
#include <QMetaEnum>

Operation::Operation(Operation::Mode mode) : m_mode(mode)
{
}

Operation::~Operation()
{
}

Operation::Mode Operation::mode() const
{
    return m_mode;
}

void Operation::run()
{
    if (!isValid()) {
        qCritical() << Q_FUNC_INFO << "INVALID:" << this;
        return;
    }

    emit started();

    operate();

    emit finished();
}

QDebug operator<<(QDebug debug, const Operation* operation)
{
    if (!operation)
        return debug << "Operation(0x0) ";

    const QMetaObject* metaObject = operation->metaObject();

    debug.nospace() << metaObject->className() << '(' << (void*) operation;
    if (!operation->objectName().isEmpty())
        debug << ", name = " << operation->objectName();

    QMetaEnum enumerator = metaObject->enumerator(metaObject->indexOfEnumerator("Mode"));
    debug << ", mode = " << enumerator.valueToKey(operation->mode())
          << ", valid = " << operation->isValid() << ')';
    return debug.space();
}
