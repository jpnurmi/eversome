isEmpty(LIBDIR):error(LIBDIR must be set)
INCLUDEPATH += $$PWD/edam/src
DEPENDPATH += $$PWD/edam/src
LIBS += -L$$LIBDIR -ledam
QMAKE_RPATHDIR += /opt/EverN9/lib
