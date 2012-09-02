isEmpty(LIBDIR):error(LIBDIR must be set)
INCLUDEPATH += $$PWD $$PWD/edam
DEPENDPATH += $$PWD/edam
LIBS += -L$$LIBDIR -ledam
PRE_TARGETDEPS += $$LIBDIR/libedam.a
