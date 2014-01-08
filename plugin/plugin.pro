TEMPLATE=lib
TARGET = nemoimagemetadataplugin
MODULENAME = org/nemomobile/imagemetadata
TARGETPATH = $$[QT_INSTALL_QML]/$$MODULENAME

QT += qml
CONFIG += qt plugin hide_symbols link_pkgconfig Qt5Qml
#PKGCONFIG += imagemetadata

INCLUDEPATH += $$PWD/../lib
LIBS += -L$$PWD/../lib -limagemetadata

SOURCES += $$PWD/plugin.cpp

OTHER_FILES += \
        $$PWD/qmldir

import.files = $$PWD/qmldir
import.path = $$TARGETPATH
target.path = $$TARGETPATH

INSTALLS += target import
