INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

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
include(3rdparty/edam.pri)
include(3rdparty/thrift.pri)
