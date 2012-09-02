TEMPLATE = subdirs

3rdparty.subdir = src/3rdparty

evern9.depends = 3rdparty
evern9.subdir = src/EverN9

SUBDIRS += 3rdparty evern9

OTHER_FILES += \
    EverN9_harmattan.desktop \
    EverN9.desktop \
    EverN9.svg \
    EverN964.png \
    EverN980.png \
    README.md \
    qtc_packaging/debian_harmattan/rules \
    qtc_packaging/debian_harmattan/README \
    qtc_packaging/debian_harmattan/manifest.aegis \
    qtc_packaging/debian_harmattan/copyright \
    qtc_packaging/debian_harmattan/control \
    qtc_packaging/debian_harmattan/compat \
    qtc_packaging/debian_harmattan/changelog
