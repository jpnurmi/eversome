TEMPLATE = subdirs

thrift.file = thrift.pro

edam.depends = thrift
edam.file = edam.pro

SUBDIRS += thrift edam
