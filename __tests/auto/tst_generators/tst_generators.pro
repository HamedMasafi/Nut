QT += testlib sql

CONFIG += qt console warn_on depend_includepath testcase
CONFIG -= app_bundle

TEMPLATE = app

include(../common/nut-lib.pri)

SOURCES += \ 
    tst_generators.cpp

HEADERS += \
    tst_generators.h
