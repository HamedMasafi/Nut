QT       += testlib sql

TARGET = tst_benchmark
TEMPLATE = app

CONFIG   += warn_on c++11

include(../common/nut-lib.pri)

SOURCES += \
    ../common/comment.cpp \
    ../common/post.cpp \
    ../common/user.cpp \
    ../common/weblogdatabase.cpp \
    ../common/score.cpp \
    tst_benchmark.cpp

HEADERS += \
    ../common/consts.h \
    ../common/comment.h \
    ../common/post.h \
    ../common/user.h \
    ../common/weblogdatabase.h \
    ../common/score.h \
    tst_benchmark.h

