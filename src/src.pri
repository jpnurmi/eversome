INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

HEADERS += \
    cache.h \
    database.h \
    fileutils.h \
    session.h \
    settings.h \
    wrappers/notebookwrapper.h \
    wrappers/notewrapper.h \
    wrappers/resourcewrapper.h \
    wrappers/tagwrapper.h

SOURCES += \
    cache.cpp \
    database.cpp \
    fileutils.cpp \
    main.cpp \
    session.cpp \
    settings.cpp \
    wrappers/notebookwrapper.cpp \
    wrappers/notewrapper.cpp \
    wrappers/resourcewrapper.cpp \
    wrappers/tagwrapper.cpp

include(edam/edam.pri)
include(thrift/thrift.pri)
