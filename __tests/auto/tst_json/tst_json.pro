QT       += testlib sql

TARGET = tst_upgrades
TEMPLATE = app
CONFIG   += warn_on c++11

include(../common/nut-lib.pri)
#include(/doc/dev/qt/Nut/src/nut/nut.pri)

SOURCES +=  \
    tst_json.cpp \
    db.cpp \
    sampletable.cpp

HEADERS += \
    tst_json.h \
    db.h \
    sampletable.h
