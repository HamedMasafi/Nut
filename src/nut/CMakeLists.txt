# Generated from nut.pro.

#####################################################################
## Nut Module:
#####################################################################

qt_internal_add_module(Nut
    SOURCES
        config/nut_config.h
        config/nut_consts.h
        config/nut_global.h
        config/nut_macros.h
        config/nut_namespace.h
        core/abstracttableset.cpp core/abstracttableset.h
        core/abstracttablesetdata.h
        core/bulkinserter.cpp core/bulkinserter.h core/bulkinserter_p.h
        core/changelogtable.cpp core/changelogtable.h
        core/database.cpp core/database.h core/database_p.h
        core/foreigncontainer.cpp core/foreigncontainer.h
        core/propertysignalmapper.cpp core/propertysignalmapper.h
        core/query.cpp core/query.h
        core/table.cpp core/table.h core/table_p.h
        core/tableset.cpp core/tableset.h
        core/sqlserializer.cpp core/sqlserializer.h
        generators/abstractsqlgenerator.cpp generators/abstractsqlgenerator.h
        generators/mysqlgenerator.cpp generators/mysqlgenerator.h
        generators/postgresqlgenerator.cpp generators/postgresqlgenerator.h
        generators/sqlitegenerator.cpp generators/sqlitegenerator.h
        generators/sqlservergenerator.cpp generators/sqlservergenerator.h
        models/databasemodel.cpp models/databasemodel.h
        models/sqlmodel.cpp models/sqlmodel.h models/sqlmodel_p.h
        models/tablemodel.cpp models/tablemodel.h
        phrase.cpp phrase.h
        phrases/abstractfieldphrase.cpp phrases/abstractfieldphrase.h
        phrases/assignmentphrase.cpp phrases/assignmentphrase.h
        phrases/assignmentphraselist.cpp phrases/assignmentphraselist.h
        phrases/conditionalphrase.cpp phrases/conditionalphrase.h
        phrases/fieldphrase.cpp phrases/fieldphrase.h
        phrases/fieldphrase_bool.cpp phrases/fieldphrase_bool.h
        phrases/fieldphrase_date.cpp phrases/fieldphrase_date.h
        phrases/fieldphrase_qstring.cpp phrases/fieldphrase_qstring.h
        phrases/phrasedata.cpp phrases/phrasedata.h
        phrases/phrasedatalist.cpp phrases/phrasedatalist.h
        phrases/phraselist.cpp phrases/phraselist.h
        types/dbgeography.cpp types/dbgeography.h
    DEFINES
        NUT_BUILD_LIB
        NUT_SHARED
        NUT_SHARED_POINTER
        QT_DEPRECATED_WARNINGS
    INCLUDE_DIRECTORIES
        config
        core
        generators
        models
        phrases
        types
    PUBLIC_LIBRARIES
        Qt::Core
        Qt::Gui
        Qt::Sql
    GENERATE_CPP_EXPORTS
)

#### Keys ignored in scope 1:.:.:nut.pro:<TRUE>:
# MODULE = "nut"
