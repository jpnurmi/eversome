#CONFIG += qdeclarative-boostable
QT += network sql xmlpatterns

CONFIG += link_pkgconfig
PKGCONFIG += accounts-qt NAccountsUI libsignon-qt

qmlfolder.source = ../../qml/eversome
qmlfolder.target = qml
resfolder.source = ../../resources
resfolder.target = .
DEPLOYMENTFOLDERS = qmlfolder resfolder

include(qmlapplicationviewer/qmlapplicationviewer.pri)
qtcAddDeployment()

INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

LIBDIR = $$OUT_PWD/../../lib
include(../3rdparty/edam.pri)
include(../3rdparty/thrift.pri)

HEADERS += \
    account.h \
    manager.h \
    session.h

SOURCES += \
    account.cpp \
    main.cpp \
    manager.cpp \
    session.cpp

include(base/base.pri)
include(cloud/cloud.pri)
include(database/database.pri)
include(filesystem/filesystem.pri)
include(model/model.pri)

OTHER_FILES += \
    eversome_harmattan.desktop \
    eversome.desktop \
    eversome.svg \
    eversome64.png \
    eversome80.png
