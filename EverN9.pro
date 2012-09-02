TEMPLATE = subdirs

3rdparty.subdir = src/3rdparty

evern9.depends = 3rdparty
evern9.subdir = src/EverN9

SUBDIRS += 3rdparty evern9

OTHER_FILES += \
    README.md \
    qtc_packaging/debian_harmattan/rules \
    qtc_packaging/debian_harmattan/README \
    qtc_packaging/debian_harmattan/manifest.aegis \
    qtc_packaging/debian_harmattan/copyright \
    qtc_packaging/debian_harmattan/control \
    qtc_packaging/debian_harmattan/compat \
    qtc_packaging/debian_harmattan/changelog
