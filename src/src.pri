INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

HEADERS += \
    cache.h \
    constants/constants.h \
    db/databasemanager.h \
    db/databaseconstants.h \
    fileutils.h \
    evernotesession.h \
    settings.h \
    wrappers/notebookwrapper.h \
    wrappers/notewrapper.h \
    wrappers/resourcewrapper.h \
    wrappers/tagwrapper.h

SOURCES += \
    cache.cpp \
    constants/constants.cpp \
    db/databasemanager.cpp \
    db/databaseconstants.cpp \
    fileutils.cpp \
    evernotesession.cpp \
    main.cpp \
    settings.cpp \
    wrappers/notebookwrapper.cpp \
    wrappers/notewrapper.cpp \
    wrappers/resourcewrapper.cpp \
    wrappers/tagwrapper.cpp

include(edam/edam.pri)
include(thrift/thrift.pri)
