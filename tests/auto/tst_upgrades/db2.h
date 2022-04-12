#ifndef DB2_H
#define DB2_H

#include "database.h"

class Table2;

class DB2 : public NUT_WRAP_NAMESPACE(Database)
{
    Q_OBJECT

    NUT_DB_VERSION(1)

    NUT_DECLARE_TABLE(Table2, sampleTable)

public:
    DB2();
};

Q_DECLARE_METATYPE(DB2*)

#endif // DB2_H
