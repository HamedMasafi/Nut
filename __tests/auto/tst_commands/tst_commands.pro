QT       += qml quick testlib sql

TARGET = tst_nut
CONFIG   += warn_on qmltestcase c++11
INCLUDEPATH += $$PWD/../../src $$PWD/../common
include(../../nut.pri)
TEMPLATE = app
IMPORTPATH += $$OUT_PWD/../src/imports
SOURCES += \
    ../common/comment.cpp \
    ../common/post.cpp \
    ../common/weblogdatabase.cpp \
    ../common/user.cpp \
    tst_commands.cpp

HEADERS += \
    ../common/comment.h \
    ../common/post.h \
    ../common/weblogdatabase.h \
    ../common/user.h \
    tst_commands.h
