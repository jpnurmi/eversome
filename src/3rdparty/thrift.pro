TEMPLATE = lib
TARGET = thrift
CONFIG -= qt
DESTDIR = ../../lib

target.path = /opt/eversome/lib
INSTALLS += target

THRIFT = $$PWD/thrift/lib/cpp/src/thrift

INCLUDEPATH += $$THRIFT
INCLUDEPATH += $$THRIFT/../
DEPENDPATH += $$THRIFT

INCLUDEPATH += $$PWD
DEFINES += HAVE_CONFIG_H

HEADERS += \
    $$THRIFT/config.h \
    $$THRIFT/TReflectionLocal.h \
    $$THRIFT/TProcessor.h \
    $$THRIFT/TLogging.h \
    $$THRIFT/Thrift.h \
    $$THRIFT/TApplicationException.h \
    $$THRIFT/async/TEvhttpClientChannel.h \
    $$THRIFT/async/TAsyncProtocolProcessor.h \
    $$THRIFT/async/TAsyncProcessor.h \
    $$THRIFT/async/TAsyncChannel.h \
    $$THRIFT/async/TAsyncBufferProcessor.h \
    $$THRIFT/concurrency/Util.h \
    $$THRIFT/concurrency/TimerManager.h \
    $$THRIFT/concurrency/ThreadManager.h \
    $$THRIFT/concurrency/Thread.h \
    $$THRIFT/concurrency/PosixThreadFactory.h \
    $$THRIFT/concurrency/Mutex.h \
    $$THRIFT/concurrency/Monitor.h \
    $$THRIFT/concurrency/FunctionRunner.h \
    $$THRIFT/concurrency/Exception.h \
    $$THRIFT/processor/StatsProcessor.h \
    $$THRIFT/processor/PeekProcessor.h \
    $$THRIFT/protocol/TVirtualProtocol.h \
    $$THRIFT/protocol/TProtocolTap.h \
    $$THRIFT/protocol/TProtocolException.h \
    $$THRIFT/protocol/TProtocol.h \
    $$THRIFT/protocol/TJSONProtocol.h \
    $$THRIFT/protocol/TDenseProtocol.h \
    $$THRIFT/protocol/TDebugProtocol.h \
    $$THRIFT/protocol/TCompactProtocol.h \
    $$THRIFT/protocol/TBinaryProtocol.h \
    $$THRIFT/protocol/TBase64Utils.h \
    $$THRIFT/transport/TZlibTransport.h \
    $$THRIFT/transport/TVirtualTransport.h \
    $$THRIFT/transport/TTransportUtils.h \
    $$THRIFT/transport/TTransportException.h \
    $$THRIFT/transport/TTransport.h \
    $$THRIFT/transport/TSSLSocket.h \
    $$THRIFT/transport/TSSLServerSocket.h \
    $$THRIFT/transport/TSocketPool.h \
    $$THRIFT/transport/TSocket.h \
    $$THRIFT/transport/TSimpleFileTransport.h \
    $$THRIFT/transport/TShortReadTransport.h \
    $$THRIFT/transport/TServerTransport.h \
    $$THRIFT/transport/TServerSocket.h \
    $$THRIFT/transport/THttpTransport.h \
    $$THRIFT/transport/THttpServer.h \
    $$THRIFT/transport/THttpClient.h \
    $$THRIFT/transport/TFileTransport.h \
    $$THRIFT/transport/TFDTransport.h \
    $$THRIFT/transport/TBufferTransports.h \

SOURCES += \
    $$THRIFT/Thrift.cpp \
    $$THRIFT/TApplicationException.cpp \
    $$THRIFT/async/TAsyncProtocolProcessor.cpp \
    $$THRIFT/async/TAsyncChannel.cpp \
    $$THRIFT/concurrency/Util.cpp \
    $$THRIFT/concurrency/TimerManager.cpp \
    $$THRIFT/concurrency/ThreadManager.cpp \
    $$THRIFT/concurrency/PosixThreadFactory.cpp \
    $$THRIFT/concurrency/Mutex.cpp \
    $$THRIFT/concurrency/Monitor.cpp \
    $$THRIFT/processor/PeekProcessor.cpp \
    $$THRIFT/protocol/TJSONProtocol.cpp \
    $$THRIFT/protocol/TDenseProtocol.cpp \
    $$THRIFT/protocol/TDebugProtocol.cpp \
    $$THRIFT/protocol/TBase64Utils.cpp \
    $$THRIFT/transport/TZlibTransport.cpp \
    $$THRIFT/transport/TTransportUtils.cpp \
    $$THRIFT/transport/TTransportException.cpp \
    $$THRIFT/transport/TSocketPool.cpp \
    $$THRIFT/transport/TSocket.cpp \
    $$THRIFT/transport/TSimpleFileTransport.cpp \
    $$THRIFT/transport/THttpTransport.cpp \
    $$THRIFT/transport/THttpServer.cpp \
    $$THRIFT/transport/THttpClient.cpp \
    $$THRIFT/transport/TFileTransport.cpp \
    $$THRIFT/transport/TFDTransport.cpp \
    $$THRIFT/transport/TBufferTransports.cpp
