QT       += testlib sql gui

TARGET = tst_datetime
TEMPLATE = app

CONFIG   += warn_on c++11

include(../common/nut-lib.pri)

SOURCES += \
    db.cpp \
    sampletable.cpp \
    tst_datetime.cpp

HEADERS += \
    db.h \
    sampletable.h \
    tst_datetime.h

