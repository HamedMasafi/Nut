QT       += testlib sql

TARGET = tst_basic
TEMPLATE = app

CONFIG   += warn_on c++11

include(../common/nut-lib.pri)

SOURCES += \
    sampledatabase.cpp \
    sampletable.cpp \
    tst_properties.cpp

HEADERS += \
    sampledatabase.h \
    sampletable.h \
    tst_properties.h

