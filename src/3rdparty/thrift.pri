isEmpty(LIBDIR):error(LIBDIR must be set)
DEFINES += HAVE_CONFIG_H
INCLUDEPATH += $$PWD
INCLUDEPATH += $$PWD/thrift/lib/cpp/src
INCLUDEPATH += $$PWD/thrift/lib/cpp/src/thrift
DEPENDPATH += $$PWD/thrift/lib/cpp/src/thrift
LIBS += -L$$LIBDIR -lthrift
QMAKE_RPATHDIR += /opt/eversome/lib
