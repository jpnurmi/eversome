TEMPLATE = lib
TARGET = edam
CONFIG -= qt
DESTDIR = ../../lib

target.path = /opt/EverN9/lib
INSTALLS += target

EVERNOTE = $$PWD/evernote/src

INCLUDEPATH += $$EVERNOTE
DEPENDPATH += $$EVERNOTE

LIBDIR = $$OUT_PWD/../../lib
include(thrift.pri)

HEADERS += \
    $$EVERNOTE/Errors_constants.h \
    $$EVERNOTE/Errors_types.h \
    $$EVERNOTE/Limits_constants.h \
    $$EVERNOTE/Limits_types.h \
    $$EVERNOTE/NoteStore.h \
    $$EVERNOTE/NoteStore_constants.h \
    $$EVERNOTE/NoteStore_types.h \
    $$EVERNOTE/Types_constants.h \
    $$EVERNOTE/Types_types.h \
    $$EVERNOTE/UserStore_types.cpp

SOURCES += \
    $$EVERNOTE/Errors_constants.cpp \
    $$EVERNOTE/Errors_types.cpp \
    $$EVERNOTE/Limits_constants.cpp \
    $$EVERNOTE/Limits_types.cpp \
    $$EVERNOTE/NoteStore.cpp \
    $$EVERNOTE/NoteStore_constants.cpp \
    $$EVERNOTE/NoteStore_types.cpp \
    $$EVERNOTE/Types_constants.cpp \
    $$EVERNOTE/Types_types.cpp \
    $$EVERNOTE/UserStore_types.cpp
