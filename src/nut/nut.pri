DEFINES += NUT_SHARED_POINTER

INCLUDEPATH += $$PWD

include(config/config.pri)
include(core/core.pri)
include(generators/generators.pri)
include(types/types.pri)
include(phrases/phrases.pri)
include(models/models.pri)
include($$PWD/3rdparty/serializer/src/src.pri)

