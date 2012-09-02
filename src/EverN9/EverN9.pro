#CONFIG += qdeclarative-boostable
QT += sql

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
    database.h \
    notestore.h \
    session.h \
    settings.h \
    userstore.h

SOURCES += \
    database.cpp \
    main.cpp \
    notestore.cpp \
    session.cpp \
    settings.cpp \
    userstore.cpp

include(model/model.pri)

OTHER_FILES += \
    EverN9_harmattan.desktop \
    EverN9.desktop \
    EverN9.svg \
    EverN964.png \
    EverN980.png
