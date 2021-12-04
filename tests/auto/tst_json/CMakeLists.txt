# Generated from tst_json.pro.

#####################################################################
## tst_upgrades Binary:
#####################################################################

qt_internal_add_executable(tst_upgrades
    GUI
    SOURCES
        ../../../src/nut/3rdparty/serializer/src/abstractserializer.cpp ../../../src/nut/3rdparty/serializer/src/abstractserializer.h
        ../../../src/nut/3rdparty/serializer/src/binaryserializer.cpp ../../../src/nut/3rdparty/serializer/src/binaryserializer.h
        ../../../src/nut/3rdparty/serializer/src/jsonserializer.cpp ../../../src/nut/3rdparty/serializer/src/jsonserializer.h
        ../../../src/nut/3rdparty/serializer/src/serializer_global.h
        ../../../src/nut/3rdparty/serializer/src/sqlserializer.cpp ../../../src/nut/3rdparty/serializer/src/sqlserializer.h
        ../../../src/nut/3rdparty/serializer/src/stringserializer.cpp ../../../src/nut/3rdparty/serializer/src/stringserializer.h
        ../../../src/nut/config/nut_config.h
        ../../../src/nut/config/nut_consts.h
        ../../../src/nut/config/nut_global.h
        ../../../src/nut/config/nut_macros.h
        ../../../src/nut/config/nut_namespace.h
        ../../../src/nut/core/abstracttableset.cpp ../../../src/nut/core/abstracttableset.h
        ../../../src/nut/core/abstracttablesetdata.h
        ../../../src/nut/core/bulkinserter.cpp ../../../src/nut/core/bulkinserter.h ../../../src/nut/core/bulkinserter_p.h
        ../../../src/nut/core/changelogtable.cpp ../../../src/nut/core/changelogtable.h
        ../../../src/nut/core/database.cpp ../../../src/nut/core/database.h ../../../src/nut/core/database_p.h
        ../../../src/nut/core/foreigncontainer.cpp ../../../src/nut/core/foreigncontainer.h
        ../../../src/nut/core/propertysignalmapper.cpp ../../../src/nut/core/propertysignalmapper.h
        ../../../src/nut/core/query.cpp ../../../src/nut/core/query.h
        ../../../src/nut/core/table.cpp ../../../src/nut/core/table.h ../../../src/nut/core/table_p.h
        ../../../src/nut/core/tableset.cpp ../../../src/nut/core/tableset.h
        ../../../src/nut/generators/abstractsqlgenerator.cpp ../../../src/nut/generators/abstractsqlgenerator.h
        ../../../src/nut/generators/mysqlgenerator.cpp ../../../src/nut/generators/mysqlgenerator.h
        ../../../src/nut/generators/postgresqlgenerator.cpp ../../../src/nut/generators/postgresqlgenerator.h
        ../../../src/nut/generators/sqlitegenerator.cpp ../../../src/nut/generators/sqlitegenerator.h
        ../../../src/nut/generators/sqlservergenerator.cpp ../../../src/nut/generators/sqlservergenerator.h
        ../../../src/nut/models/databasemodel.cpp ../../../src/nut/models/databasemodel.h
        ../../../src/nut/models/sqlmodel.cpp ../../../src/nut/models/sqlmodel.h ../../../src/nut/models/sqlmodel_p.h
        ../../../src/nut/models/tablemodel.cpp ../../../src/nut/models/tablemodel.h
        ../../../src/nut/phrase.cpp ../../../src/nut/phrase.h
        ../../../src/nut/phrases/abstractfieldphrase.cpp ../../../src/nut/phrases/abstractfieldphrase.h
        ../../../src/nut/phrases/assignmentphrase.cpp ../../../src/nut/phrases/assignmentphrase.h
        ../../../src/nut/phrases/assignmentphraselist.cpp ../../../src/nut/phrases/assignmentphraselist.h
        ../../../src/nut/phrases/conditionalphrase.cpp ../../../src/nut/phrases/conditionalphrase.h
        ../../../src/nut/phrases/fieldphrase.cpp ../../../src/nut/phrases/fieldphrase.h
        ../../../src/nut/phrases/fieldphrase_bool.cpp ../../../src/nut/phrases/fieldphrase_bool.h
        ../../../src/nut/phrases/fieldphrase_date.cpp ../../../src/nut/phrases/fieldphrase_date.h
        ../../../src/nut/phrases/fieldphrase_qstring.cpp ../../../src/nut/phrases/fieldphrase_qstring.h
        ../../../src/nut/phrases/phrasedata.cpp ../../../src/nut/phrases/phrasedata.h
        ../../../src/nut/phrases/phrasedatalist.cpp ../../../src/nut/phrases/phrasedatalist.h
        ../../../src/nut/phrases/phraselist.cpp ../../../src/nut/phrases/phraselist.h
        ../../../src/nut/types/dbgeography.cpp ../../../src/nut/types/dbgeography.h
        db.cpp db.h
        sampletable.cpp sampletable.h
        tst_json.cpp tst_json.h
    DEFINES
        NUT_SHARED_POINTER
    INCLUDE_DIRECTORIES
        ../../../src/nut
        ../../../src/nut/3rdparty/serializer/src
        ../../../src/nut/config
        ../../../src/nut/core
        ../../../src/nut/generators
        ../../../src/nut/models
        ../../../src/nut/phrases
        ../../../src/nut/types
    PUBLIC_LIBRARIES
        Qt::Gui
        Qt::Sql
)

#### Keys ignored in scope 1:.:.:tst_json.pro:<TRUE>:
# TEMPLATE = "app"
