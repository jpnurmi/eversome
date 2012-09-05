TEMPLATE = subdirs

thrift.file = thrift.pro

evernote.depends = thrift
evernote.file = evernote.pro

SUBDIRS += thrift evernote
