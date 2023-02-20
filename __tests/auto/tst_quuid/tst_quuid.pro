QT       += testlib sql

TARGET = tst_uuid
TEMPLATE = app
CONFIG   += warn_on c++11

include(../common/nut-lib.pri)

SOURCES += \
    testdatabase.cpp \
    test.cpp \
    tst_uuid.cpp

HEADERS += \
    ../common/consts.h \
    testdatabase.h \
    test.h \
    tst_uuid.h

