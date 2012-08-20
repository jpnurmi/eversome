# Add more folders to ship with the application, here
folder_01.source = qml/Evernote
folder_01.target = qml
DEPLOYMENTFOLDERS = folder_01

# Additional import path used to resolve QML modules in Creator's code model
QML_IMPORT_PATH =

symbian:TARGET.UID3 = 0xE10B81B8

# Smart Installer package's UID
# This UID is from the protected range and therefore the package will
# fail to install if self-signed. By default qmake uses the unprotected
# range value if unprotected UID is defined for the application and
# 0x2002CCCF value if protected UID is given to the application
#symbian:DEPLOYMENT.installer_header = 0x2002CCCF

# Allow network access on Symbian
symbian:TARGET.CAPABILITY += NetworkServices

# If your application uses the Qt Mobility libraries, uncomment the following
# lines and add the respective components to the MOBILITY variable.
# CONFIG += mobility
# MOBILITY +=

# Speed up launching on MeeGo/Harmattan when using applauncherd daemon
CONFIG += qdeclarative-boostable
QT += sql
# Add dependency to Symbian components
# CONFIG += qt-components

INCLUDEPATH += thrift
DEFINES += HAVE_CONFIG_H

# The .cpp file which was generated for your project. Feel free to hack it.
SOURCES += main.cpp \
    edam/UserStore_types.cpp \
    edam/UserStore_constants.cpp \
    edam/UserStore.cpp \
    edam/Types_types.cpp \
    edam/Types_constants.cpp \
    edam/NoteStore_types.cpp \
    edam/NoteStore_constants.cpp \
    edam/NoteStore.cpp \
    edam/Limits_types.cpp \
    edam/Limits_constants.cpp \
    edam/Errors_types.cpp \
    edam/Errors_constants.cpp \
    thrift/Thrift.cpp \
    thrift/TApplicationException.cpp \
    thrift/async/TAsyncProtocolProcessor.cpp \
    thrift/async/TAsyncChannel.cpp \
    thrift/concurrency/Util.cpp \
    thrift/concurrency/TimerManager.cpp \
    thrift/concurrency/ThreadManager.cpp \
    thrift/concurrency/PosixThreadFactory.cpp \
    thrift/concurrency/Mutex.cpp \
    thrift/concurrency/Monitor.cpp \
    thrift/processor/PeekProcessor.cpp \
    thrift/protocol/TJSONProtocol.cpp \
    thrift/protocol/TDenseProtocol.cpp \
    thrift/protocol/TDebugProtocol.cpp \
    thrift/protocol/TCompactProtocol.tcc \
    thrift/protocol/TBinaryProtocol.tcc \
    thrift/protocol/TBase64Utils.cpp \
    thrift/transport/TZlibTransport.cpp \
    thrift/transport/TTransportUtils.cpp \
    thrift/transport/TTransportException.cpp \
    thrift/transport/TSocketPool.cpp \
    thrift/transport/TSocket.cpp \
    thrift/transport/TSimpleFileTransport.cpp \
    thrift/transport/THttpTransport.cpp \
    thrift/transport/THttpServer.cpp \
    thrift/transport/THttpClient.cpp \
    thrift/transport/TFileTransport.cpp \
    thrift/transport/TFDTransport.cpp \
    thrift/transport/TBufferTransports.cpp \
    evernotesession.cpp \
    constants/constants.cpp \
    db/databasemanager.cpp \
    db/databaseconstants.cpp \
    constants/settingskeys.cpp \
    settings.cpp \
    cache.cpp \
    wrappers/tagwrapper.cpp \
    wrappers/notewrapper.cpp \
    fileutils.cpp \
    wrappers/resourcewrapper.cpp \
    wrappers/notebookwrapper.cpp


# Please do not modify the following two lines. Required for deployment.
include(qmlapplicationviewer/qmlapplicationviewer.pri)
qtcAddDeployment()

HEADERS += \
    edam/UserStore_types.h \
    edam/UserStore_constants.h \
    edam/UserStore.h \
    edam/Types_types.h \
    edam/Types_constants.h \
    edam/NoteStore_types.h \
    edam/NoteStore_constants.h \
    edam/NoteStore.h \
    edam/Limits_types.h \
    edam/Limits_constants.h \
    edam/Errors_types.h \
    edam/Errors_constants.h \
    thrift/TReflectionLocal.h \
    thrift/TProcessor.h \
    thrift/TLogging.h \
    thrift/Thrift.h \
    thrift/TApplicationException.h \
    thrift/async/TEvhttpClientChannel.h \
    thrift/async/TAsyncProtocolProcessor.h \
    thrift/async/TAsyncProcessor.h \
    thrift/async/TAsyncChannel.h \
    thrift/async/TAsyncBufferProcessor.h \
    thrift/concurrency/Util.h \
    thrift/concurrency/TimerManager.h \
    thrift/concurrency/ThreadManager.h \
    thrift/concurrency/Thread.h \
    thrift/concurrency/PosixThreadFactory.h \
    thrift/concurrency/Mutex.h \
    thrift/concurrency/Monitor.h \
    thrift/concurrency/FunctionRunner.h \
    thrift/concurrency/Exception.h \
    thrift/processor/StatsProcessor.h \
    thrift/processor/PeekProcessor.h \
    thrift/protocol/TVirtualProtocol.h \
    thrift/protocol/TProtocolTap.h \
    thrift/protocol/TProtocolException.h \
    thrift/protocol/TProtocol.h \
    thrift/protocol/TJSONProtocol.h \
    thrift/protocol/TDenseProtocol.h \
    thrift/protocol/TDebugProtocol.h \
    thrift/protocol/TCompactProtocol.h \
    thrift/protocol/TBinaryProtocol.h \
    thrift/protocol/TBase64Utils.h \
    thrift/transport/TZlibTransport.h \
    thrift/transport/TVirtualTransport.h \
    thrift/transport/TTransportUtils.h \
    thrift/transport/TTransportException.h \
    thrift/transport/TTransport.h \
    thrift/transport/TSSLSocket.h \
    thrift/transport/TSSLServerSocket.h \
    thrift/transport/TSocketPool.h \
    thrift/transport/TSocket.h \
    thrift/transport/TSimpleFileTransport.h \
    thrift/transport/TShortReadTransport.h \
    thrift/transport/TServerTransport.h \
    thrift/transport/TServerSocket.h \
    thrift/transport/THttpTransport.h \
    thrift/transport/THttpServer.h \
    thrift/transport/THttpClient.h \
    thrift/transport/TFileTransport.h \
    thrift/transport/TFDTransport.h \
    thrift/transport/TBufferTransports.h \
    evernotesession.h \
    constants/constants.h \
    db/databasemanager.h \
    db/databaseconstants.h \
    constants/settingskeys.h \
    settings.h \
    cache.h \
    wrappers/tagwrapper.h \
    wrappers/notewrapper.h \
    fileutils.h \
    wrappers/resourcewrapper.h \
    wrappers/notebookwrapper.h



OTHER_FILES += \
    qtc_packaging/debian_harmattan/rules \
    qtc_packaging/debian_harmattan/README \
    qtc_packaging/debian_harmattan/manifest.aegis \
    qtc_packaging/debian_harmattan/copyright \
    qtc_packaging/debian_harmattan/control \
    qtc_packaging/debian_harmattan/compat \
    qtc_packaging/debian_harmattan/changelog \
    images/login_icon.png \
    images/login_bg.png \
    evernote.ru.ts \
    evernote.ru.qm \
    evernote.en.ts \
    evernote.en.qm \
    qml/EditNotePage.qml

RESOURCES += \
    evernote.qrc
















































