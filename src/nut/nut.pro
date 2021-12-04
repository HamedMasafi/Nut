load(qt_build_config)
MODULE = nut

TARGET = QtNut

QT = core sql gui

DEFINES += QT_DEPRECATED_WARNINGS NUT_SHARED NUT_BUILD_LIB

DEFINES += NUT_SHARED_POINTER

include(config/config.pri)
include(core/core.pri)
include(generators/generators.pri)
include(types/types.pri)
include(phrases/phrases.pri)
include(models/models.pri)

HEADERS += \
    $$PWD/phrase.h \
    $$PWD/nut_p.h

SOURCES += \
    $$PWD/phrase.cpp

load(qt_module)

