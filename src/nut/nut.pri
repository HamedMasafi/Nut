DEFINES += NUT_SHARED_POINTER
QT = core sql gui

INCLUDEPATH += $$PWD

include(config/config.pri)
include(core/core.pri)
include(generators/generators.pri)
include(types/types.pri)
include(phrases/phrases.pri)
include(models/models.pri)

HEADERS += \
    $$PWD/phrase.h

SOURCES += \
    $$PWD/phrase.cpp

