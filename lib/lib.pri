QT += qml
CONFIG += link_pkgconfig
PKGCONFIG += exiv2

HEADERS += \
    $$PWD/exifdata.h \
    $$PWD/exifdata_p.h \
    $$PWD/imagedata.h \
    $$PWD/iptcdata.h \
    $$PWD/xmpdata.h

SOURCES += \
    $$PWD/exifdata.cpp \
    $$PWD/imagedata.cpp \
    $$PWD/iptcdata.cpp \
    $$PWD/xmpdata.cpp
