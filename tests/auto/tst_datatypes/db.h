#ifndef DB_H
#define DB_H

#include "database.h"

class SampleTable;
class DB : public NUT_WRAP_NAMESPACE(Database)
{
    Q_OBJECT

    NUT_DB_VERSION(1)

    NUT_DECLARE_TABLE(SampleTable, sampleTables)

public:
    DB();
};

Q_DECLARE_METATYPE(DB*)

#endif // DB_H
