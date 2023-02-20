QT       += testlib sql

TARGET = tst_phrases
TEMPLATE = app

CONFIG   += warn_on c++11

include(../common/nut-lib.pri)

SOURCES += \
    generator.cpp \
    tst_phrases.cpp

HEADERS += \
    generator.h \
    tst_phrases.h

