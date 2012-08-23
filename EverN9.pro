qmlfolder.source = qml/EverN9
qmlfolder.target = qml
DEPLOYMENTFOLDERS = qmlfolder

#CONFIG += qdeclarative-boostable
QT += sql

include(src/src.pri)
include(qmlapplicationviewer/qmlapplicationviewer.pri)
qtcAddDeployment()

OTHER_FILES += \
    qtc_packaging/debian_harmattan/rules \
    qtc_packaging/debian_harmattan/README \
    qtc_packaging/debian_harmattan/manifest.aegis \
    qtc_packaging/debian_harmattan/copyright \
    qtc_packaging/debian_harmattan/control \
    qtc_packaging/debian_harmattan/compat \
    qtc_packaging/debian_harmattan/changelog \
    translations/EverN9.ru.ts \
    translations/EverN9.ru.qm \
    translations/EverN9.en.ts \
    translations/EverN9.en.qm

RESOURCES += \
    EverN9.qrc
