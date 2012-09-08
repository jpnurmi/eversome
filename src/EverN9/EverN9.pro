#CONFIG += qdeclarative-boostable
QT += sql

CONFIG += link_pkgconfig
PKGCONFIG += accounts-qt libsignon-qt

qmlfolder.source = ../../qml/EverN9
qmlfolder.target = qml
DEPLOYMENTFOLDERS = qmlfolder

include(qmlapplicationviewer/qmlapplicationviewer.pri)
qtcAddDeployment()

INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

LIBDIR = $$OUT_PWD/../../lib
include(../3rdparty/evernote.pri)
include(../3rdparty/thrift.pri)

HEADERS += \
    database.h \
    manager.h \
    notestore.h \
    resourcewriter.h \
    session.h

SOURCES += \
    database.cpp \
    main.cpp \
    manager.cpp \
    notestore.cpp \
    resourcewriter.cpp \
    session.cpp

include(model/model.pri)
include(operations/operations.pri)

OTHER_FILES += \
    EverN9_harmattan.desktop \
    EverN9.desktop \
    EverN9.svg \
    EverN964.png \
    EverN980.png
