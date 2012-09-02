TEMPLATE = lib
TARGET = thrift
CONFIG += static
CONFIG -= qt
DESTDIR = ../../lib

INCLUDEPATH += $$PWD $$PWD/thrift
DEPENDPATH += $$PWD/thrift
DEFINES += HAVE_CONFIG_H

HEADERS += \
    $$PWD/thrift/config.h \
    $$PWD/thrift/TReflectionLocal.h \
    $$PWD/thrift/TProcessor.h \
    $$PWD/thrift/TLogging.h \
    $$PWD/thrift/Thrift.h \
    $$PWD/thrift/TApplicationException.h \
    $$PWD/thrift/async/TEvhttpClientChannel.h \
    $$PWD/thrift/async/TAsyncProtocolProcessor.h \
    $$PWD/thrift/async/TAsyncProcessor.h \
    $$PWD/thrift/async/TAsyncChannel.h \
    $$PWD/thrift/async/TAsyncBufferProcessor.h \
    $$PWD/thrift/concurrency/Util.h \
    $$PWD/thrift/concurrency/TimerManager.h \
    $$PWD/thrift/concurrency/ThreadManager.h \
    $$PWD/thrift/concurrency/Thread.h \
    $$PWD/thrift/concurrency/PosixThreadFactory.h \
    $$PWD/thrift/concurrency/Mutex.h \
    $$PWD/thrift/concurrency/Monitor.h \
    $$PWD/thrift/concurrency/FunctionRunner.h \
    $$PWD/thrift/concurrency/Exception.h \
    $$PWD/thrift/processor/StatsProcessor.h \
    $$PWD/thrift/processor/PeekProcessor.h \
    $$PWD/thrift/protocol/TVirtualProtocol.h \
    $$PWD/thrift/protocol/TProtocolTap.h \
    $$PWD/thrift/protocol/TProtocolException.h \
    $$PWD/thrift/protocol/TProtocol.h \
    $$PWD/thrift/protocol/TJSONProtocol.h \
    $$PWD/thrift/protocol/TDenseProtocol.h \
    $$PWD/thrift/protocol/TDebugProtocol.h \
    $$PWD/thrift/protocol/TCompactProtocol.h \
    $$PWD/thrift/protocol/TBinaryProtocol.h \
    $$PWD/thrift/protocol/TBase64Utils.h \
    $$PWD/thrift/transport/TZlibTransport.h \
    $$PWD/thrift/transport/TVirtualTransport.h \
    $$PWD/thrift/transport/TTransportUtils.h \
    $$PWD/thrift/transport/TTransportException.h \
    $$PWD/thrift/transport/TTransport.h \
    $$PWD/thrift/transport/TSSLSocket.h \
    $$PWD/thrift/transport/TSSLServerSocket.h \
    $$PWD/thrift/transport/TSocketPool.h \
    $$PWD/thrift/transport/TSocket.h \
    $$PWD/thrift/transport/TSimpleFileTransport.h \
    $$PWD/thrift/transport/TShortReadTransport.h \
    $$PWD/thrift/transport/TServerTransport.h \
    $$PWD/thrift/transport/TServerSocket.h \
    $$PWD/thrift/transport/THttpTransport.h \
    $$PWD/thrift/transport/THttpServer.h \
    $$PWD/thrift/transport/THttpClient.h \
    $$PWD/thrift/transport/TFileTransport.h \
    $$PWD/thrift/transport/TFDTransport.h \
    $$PWD/thrift/transport/TBufferTransports.h \

SOURCES += \
    $$PWD/thrift/Thrift.cpp \
    $$PWD/thrift/TApplicationException.cpp \
    $$PWD/thrift/async/TAsyncProtocolProcessor.cpp \
    $$PWD/thrift/async/TAsyncChannel.cpp \
    $$PWD/thrift/concurrency/Util.cpp \
    $$PWD/thrift/concurrency/TimerManager.cpp \
    $$PWD/thrift/concurrency/ThreadManager.cpp \
    $$PWD/thrift/concurrency/PosixThreadFactory.cpp \
    $$PWD/thrift/concurrency/Mutex.cpp \
    $$PWD/thrift/concurrency/Monitor.cpp \
    $$PWD/thrift/processor/PeekProcessor.cpp \
    $$PWD/thrift/protocol/TJSONProtocol.cpp \
    $$PWD/thrift/protocol/TDenseProtocol.cpp \
    $$PWD/thrift/protocol/TDebugProtocol.cpp \
    $$PWD/thrift/protocol/TBase64Utils.cpp \
    $$PWD/thrift/transport/TZlibTransport.cpp \
    $$PWD/thrift/transport/TTransportUtils.cpp \
    $$PWD/thrift/transport/TTransportException.cpp \
    $$PWD/thrift/transport/TSocketPool.cpp \
    $$PWD/thrift/transport/TSocket.cpp \
    $$PWD/thrift/transport/TSimpleFileTransport.cpp \
    $$PWD/thrift/transport/THttpTransport.cpp \
    $$PWD/thrift/transport/THttpServer.cpp \
    $$PWD/thrift/transport/THttpClient.cpp \
    $$PWD/thrift/transport/TFileTransport.cpp \
    $$PWD/thrift/transport/TFDTransport.cpp \
    $$PWD/thrift/transport/TBufferTransports.cpp
