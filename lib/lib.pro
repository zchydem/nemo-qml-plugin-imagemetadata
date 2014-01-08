TEMPLATE = lib
TARGET = $$qtLibraryTarget(imagemetadata)
TARGETPATH = $$[QT_INSTALL_LIBS]

INCLUDEPATH += .
CONFIG += qt hide_symbols create_pc create_prl no_install_prl

include(lib.pri)

develheaders.path = /usr/include/imagemetadata
develheaders.files = $$HEADERS

target.path = $$[QT_INSTALL_LIBS]
pkgconfig.files = $$PWD/pkgconfig/imagemetadata.pc
pkgconfig.path = $$target.path/pkgconfig

QMAKE_PKGCONFIG_NAME = lib$$TARGET
QMAKE_PKGCONFIG_DESCRIPTION = Nemomobile image metadata handling library
QMAKE_PKGCONFIG_LIBDIR = $$target.path
QMAKE_PKGCONFIG_INCDIR = $$develheaders.path
QMAKE_PKGCONFIG_DESTDIR = pkgconfig
QMAKE_PKGCONFIG_REQUIRES = Qt5Qml

INSTALLS += target develheaders pkgconfig

