isEmpty(LIBDIR):error(LIBDIR must be set)
INCLUDEPATH += $$PWD $$PWD/edam
DEPENDPATH += $$PWD/edam
LIBS += -L$$LIBDIR -ledam
QMAKE_RPATHDIR += /opt/EverN9/lib
