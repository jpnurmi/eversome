INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD
DEFINES += HAVE_CONFIG_H

HEADERS += \
    thrift/config.h \
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

SOURCES += \
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
    thrift/transport/TBufferTransports.cpp
