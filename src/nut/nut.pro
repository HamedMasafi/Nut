load(qt_build_config)
MODULE = nut

TARGET = QtNut

QT = core sql gui

DEFINES += QT_DEPRECATED_WARNINGS NUT_SHARED NUT_BUILD_LIB

DEFINES += NUT_SHARED_POINTER

INCLUDEPATH +=  \
        $$PWD/generators \
        $$PWD/phrases \
        $$PWD/types

HEADERS += \
    $$PWD/generators/abstractsqlgenerator.h \
    $$PWD/generators/postgresqlgenerator.h \
    $$PWD/generators/mysqlgenerator.h \
    $$PWD/generators/sqlitegenerator.h \
    $$PWD/generators/sqlservergenerator.h \
    $$PWD/types/dbgeography.h \
    $$PWD/tableset.h \
    $$PWD/defines_consts.h \
    $$PWD/defines.h \
    $$PWD/query.h \
    $$PWD/bulkinserter.h \
    $$PWD/databasemodel.h \
    $$PWD/changelogtable.h \
    $$PWD/abstracttableset.h \
    $$PWD/abstracttablesetdata.h \
    $$PWD/tablemodel.h \
    $$PWD/table.h \
    $$PWD/database.h \
    $$PWD/database_p.h \
    $$PWD/serializableobject.h \
    $$PWD/sqlmodel.h \
    $$PWD/sqlmodel_p.h \
    $$PWD/phrase.h \
    $$PWD/phrases/abstractfieldphrase.h \
    $$PWD/phrases/assignmentphrase.h \
    $$PWD/phrases/assignmentphraselist.h \
    $$PWD/phrases/conditionalphrase.h \
    $$PWD/phrases/fieldphrase.h \
    $$PWD/phrases/phrasedata.h \
    $$PWD/phrases/phrasedatalist.h \
    $$PWD/phrases/phraselist.h \
    $$PWD/phrases/datephrase.h \
    $$PWD/table_p.h \
    bulkinserter_p.h

SOURCES += \
    $$PWD/generators/abstractsqlgenerator.cpp \
    $$PWD/generators/postgresqlgenerator.cpp \
    $$PWD/generators/mysqlgenerator.cpp \
    $$PWD/generators/sqlitegenerator.cpp \
    $$PWD/generators/sqlservergenerator.cpp \
    $$PWD/types/dbgeography.cpp \
    $$PWD/tableset.cpp \
    $$PWD/query.cpp \
    $$PWD/bulkinserter.cpp \
    $$PWD/databasemodel.cpp \
    $$PWD/abstracttableset.cpp \
    $$PWD/changelogtable.cpp \
    $$PWD/tablemodel.cpp \
    $$PWD/table.cpp \
    $$PWD/database.cpp \
    $$PWD/serializableobject.cpp \
    $$PWD/sqlmodel.cpp \
    $$PWD/phrase.cpp \
    $$PWD/phrases/abstractfieldphrase.cpp \
    $$PWD/phrases/assignmentphrase.cpp \
    $$PWD/phrases/assignmentphraselist.cpp \
    $$PWD/phrases/conditionalphrase.cpp \
    $$PWD/phrases/fieldphrase.cpp \
    $$PWD/phrases/phrasedata.cpp \
    $$PWD/phrases/phrasedatalist.cpp \
    $$PWD/phrases/phraselist.cpp \
    $$PWD/phrases/datephrase.cpp

load(qt_module)

!exists($$PWD/3rdparty/serializer/src/src.pri) {
    error("Please do git submodule update --init --recursive in the Nut directory")
}
include($$PWD/3rdparty/serializer/src/src.pri)
