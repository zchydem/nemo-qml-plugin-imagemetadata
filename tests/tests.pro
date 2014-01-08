TEMPLATE = app
TARGET = ut_imagemetadata
DEPENDPATH += .
INCLUDEPATH += . ../lib

PACKAGENAME=nemo-qml-plugin-imagemetadata

include(../lib/lib.pri)

HEADERS += \
    ut_imagedata.h \
    ut_exifdata.h \
    ut_iptcdata.h \
    ut_xmpdata.h

SOURCES += \
    main.cpp \
    ut_imagedata.cpp \
    ut_exifdata.cpp \
    ut_iptcdata.cpp \
    ut_xmpdata.cpp

QT += testlib

PATH = /opt/tests/$${PACKAGENAME}

tests_xml.target = tests.xml
tests_xml.depends = $$PWD/tests.xml.in
tests_xml.commands = sed -e "s:@PACKAGENAME@:$${PACKAGENAME}:g" $< > $@

QMAKE_EXTRA_TARGETS = tests_xml
QMAKE_CLEAN += $$tests_xml.target
PRE_TARGETDEPS += $$tests_xml.target

tests_install.depends = tests_xml
tests_install.path = $$PATH
tests_install.files = $$tests_xml.target
tests_install.CONFIG += no_check_exist

resources.files = images/*
resources.path = $$PATH/images

target.path = $$PATH

INSTALLS += target resources tests_install tests_xml
