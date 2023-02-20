QT       += qml quick testlib sql

TARGET = tst_nut
TEMPLATE = app

CONFIG   += warn_on qmltestcase c++11
INCLUDEPATH += $$PWD/../../src $$PWD/../common
include(../../nut.pri)
IMPORTPATH += $$OUT_PWD/../src/imports
SOURCES += \
    jointest.cpp \
    ../common/comment.cpp \
    ../common/post.cpp \
    ../common/user.cpp \
    ../common/weblogdatabase.cpp \
    ../common/score.cpp

HEADERS += \
    jointest.h \
    ../common/consts.h \
    ../common/comment.h \
    ../common/post.h \
    ../common/user.h \
    ../common/weblogdatabase.h \
    ../common/score.h
