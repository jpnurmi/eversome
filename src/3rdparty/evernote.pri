isEmpty(LIBDIR):error(LIBDIR must be set)
INCLUDEPATH += $$PWD/evernote/src
DEPENDPATH += $$PWD/evernote/src
LIBS += -L$$LIBDIR -levernote
QMAKE_RPATHDIR += /opt/EverN9/lib
