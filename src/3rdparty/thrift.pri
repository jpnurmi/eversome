isEmpty(LIBDIR):error(LIBDIR must be set)
DEFINES += HAVE_CONFIG_H
INCLUDEPATH += $$PWD/thrift
DEPENDPATH += $$PWD/thrift
LIBS += -L$$LIBDIR -lthrift
PRE_TARGETDEPS += $$LIBDIR/libthrift.a
