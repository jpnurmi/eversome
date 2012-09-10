#CONFIG += qdeclarative-boostable
QT += sql

CONFIG += link_pkgconfig
PKGCONFIG += accounts-qt NAccountsUI libsignon-qt

qmlfolder.source = ../../qml/EverN9
qmlfolder.target = qml
DEPLOYMENTFOLDERS = qmlfolder

include(qmlapplicationviewer/qmlapplicationviewer.pri)
qtcAddDeployment()

INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

LIBDIR = $$OUT_PWD/../../lib
include(../3rdparty/edam.pri)
include(../3rdparty/thrift.pri)

HEADERS += \
    account.h \
    database.h \
    filesystem.h \
    manager.h \
    notestore.h \
    session.h

SOURCES += \
    account.cpp \
    database.cpp \
    filesystem.cpp \
    main.cpp \
    manager.cpp \
    notestore.cpp \
    session.cpp

include(model/model.pri)
include(operations/operations.pri)

OTHER_FILES += \
    EverN9_harmattan.desktop \
    EverN9.desktop \
    EverN9.svg \
    EverN964.png \
    EverN980.png
