#CONFIG += qdeclarative-boostable
QT += sql

qmlfolder.source = ../qml/EverN9
qmlfolder.target = qml
DEPLOYMENTFOLDERS = qmlfolder

include(qmlapplicationviewer/qmlapplicationviewer.pri)
qtcAddDeployment()

INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

LIBDIR = $$OUT_PWD/../lib
include(3rdparty/edam.pri)
include(3rdparty/thrift.pri)

HEADERS += \
    authenticator.h \
    database.h \
    session.h \
    settings.h \
    synchronizer.h

SOURCES += \
    authenticator.cpp \
    database.cpp \
    main.cpp \
    session.cpp \
    settings.cpp \
    synchronizer.cpp

include(model/model.pri)
