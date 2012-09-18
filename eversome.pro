TEMPLATE = subdirs

3rdparty.subdir = src/3rdparty

eversome.depends = 3rdparty
eversome.subdir = src/eversome

SUBDIRS += 3rdparty eversome

OTHER_FILES += \
    README.md \
    qtc_packaging/debian_harmattan/rules \
    qtc_packaging/debian_harmattan/README \
    qtc_packaging/debian_harmattan/manifest.aegis \
    qtc_packaging/debian_harmattan/copyright \
    qtc_packaging/debian_harmattan/control \
    qtc_packaging/debian_harmattan/compat \
    qtc_packaging/debian_harmattan/changelog \
    qtc_packaging/debian_harmattan/rules \
    qtc_packaging/debian_harmattan/README \
    qtc_packaging/debian_harmattan/manifest.aegis \
    qtc_packaging/debian_harmattan/copyright \
    qtc_packaging/debian_harmattan/control \
    qtc_packaging/debian_harmattan/compat \
    qtc_packaging/debian_harmattan/changelog

THRIFT = $$files(src/3rdparty/thrift/*)
isEmpty(THRIFT):error(The thrift submodule is missing.  \
                      Run \'git submodule init\' and \
                      \'git submodule update\' in $$_PRO_FILE_PWD_)

EDAM = $$files(src/3rdparty/edam/*)
isEmpty(EDAM):error(The edam submodule is missing.  \
                    Run \'git submodule init\' and \
                    \'git submodule update\' in $$_PRO_FILE_PWD_)
