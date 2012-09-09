TEMPLATE = lib
TARGET = edam
CONFIG -= qt
DESTDIR = ../../lib

target.path = /opt/EverN9/lib
INSTALLS += target

EDAM = $$PWD/edam/src

INCLUDEPATH += $$EDAM
DEPENDPATH += $$EDAM

LIBDIR = $$OUT_PWD/../../lib
include(thrift.pri)

HEADERS += \
    $$EDAM/Errors_constants.h \
    $$EDAM/Errors_types.h \
    $$EDAM/Limits_constants.h \
    $$EDAM/Limits_types.h \
    $$EDAM/NoteStore.h \
    $$EDAM/NoteStore_constants.h \
    $$EDAM/NoteStore_types.h \
    $$EDAM/Types_constants.h \
    $$EDAM/Types_types.h \
    $$EDAM/UserStore_types.cpp

SOURCES += \
    $$EDAM/Errors_constants.cpp \
    $$EDAM/Errors_types.cpp \
    $$EDAM/Limits_constants.cpp \
    $$EDAM/Limits_types.cpp \
    $$EDAM/NoteStore.cpp \
    $$EDAM/NoteStore_constants.cpp \
    $$EDAM/NoteStore_types.cpp \
    $$EDAM/Types_constants.cpp \
    $$EDAM/Types_types.cpp \
    $$EDAM/UserStore_types.cpp
