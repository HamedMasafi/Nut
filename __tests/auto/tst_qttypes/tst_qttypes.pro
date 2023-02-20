QT += testlib sql gui

CONFIG += qt console warn_on depend_includepath testcase
CONFIG -= app_bundle

TEMPLATE = app

SOURCES +=  tst_qttypes.cpp \
    db.cpp \
    sampletable.cpp

include(../common/nut-lib.pri)

HEADERS += \
    db.h \
    sampletable.h \
    tst_qttypes.h
